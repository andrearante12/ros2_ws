#include <memory>
#include <mutex>
#include <fstream>
#include <iomanip>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <nav_msgs/msg/odometry.hpp>

#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/robot_model_loader/robot_model_loader.hpp>
#include <moveit/robot_state/robot_state.hpp>

// =======================
//  Gradient Descent IK Solver
// =======================
class GradientDescentIK {
private:
    moveit::core::RobotStatePtr robot_state_;
    const moveit::core::JointModelGroup* joint_model_group_;
    std::string end_effector_link_;
    rclcpp::Logger logger_;
    
public:
    GradientDescentIK(
        const moveit::core::RobotModelPtr& robot_model,
        const std::string& group_name,
        const std::string& ee_link
    ) : logger_(rclcpp::get_logger("gradient_ik")) {
        robot_state_ = std::make_shared<moveit::core::RobotState>(robot_model);
        joint_model_group_ = robot_model->getJointModelGroup(group_name);
        end_effector_link_ = ee_link;
    }
    
    double calculateError(const Eigen::Vector3d& target_pos) {
        const Eigen::Isometry3d& current_pose = 
            robot_state_->getGlobalLinkTransform(end_effector_link_);
        
        Eigen::Vector3d current_pos = current_pose.translation();
        return (target_pos - current_pos).squaredNorm();
    }
    
    std::vector<double> computeGradient(
        const Eigen::Vector3d& target_pos,
        const std::vector<double>& joint_values,
        double epsilon = 0.001
    ) {
        std::vector<double> gradient(joint_values.size());
        for (size_t i = 0; i < joint_values.size(); i++) {
            std::vector<double> perturbed = joint_values;
            perturbed[i] += epsilon;
            robot_state_->setJointGroupPositions(joint_model_group_, perturbed);
            double error_plus = calculateError(target_pos);
            
            perturbed[i] = joint_values[i] - epsilon;
            robot_state_->setJointGroupPositions(joint_model_group_, perturbed);
            double error_minus = calculateError(target_pos);
            
            gradient[i] = (error_plus - error_minus) / (2.0 * epsilon);
        }
        return gradient;
    }
    
    bool solveIK(
        const Eigen::Vector3d& target_pos,
        std::vector<double>& solution,
        double learning_rate = 0.05,
        int max_iterations = 5000,
        double tolerance = 0.01
    ) {
        std::vector<double> joint_values = solution;
        double prev_error = std::numeric_limits<double>::max();
        int stall_count = 0;
        
        for (int iter = 0; iter < max_iterations; iter++) {
            robot_state_->setJointGroupPositions(joint_model_group_, joint_values);
            double error = std::sqrt(calculateError(target_pos));
            
            if (error < tolerance) {
                solution = joint_values;
                return true;
            }
            
            if (std::abs(prev_error - error) < 1e-6) {
                if (++stall_count > 50) break;
            } else {
                stall_count = 0;
            }
            prev_error = error;
            
            auto gradient = computeGradient(target_pos, joint_values);
            for (size_t i = 0; i < joint_values.size(); i++) {
                joint_values[i] -= learning_rate * gradient[i];
                const auto& bounds = joint_model_group_->getActiveJointModels()[i]->getVariableBounds();
                joint_values[i] = std::clamp(joint_values[i], bounds[0].min_position_, bounds[0].max_position_);
            }
        }
        solution = joint_values;
        return std::sqrt(calculateError(target_pos)) < 0.05;
    }
};

// =======================
//  Serial Port Communication
// =======================
class SerialPort {
private:
    int fd_;
    rclcpp::Logger logger_;
    
public:
    SerialPort(const std::string& port, int baudrate) 
        : fd_(-1), logger_(rclcpp::get_logger("serial_port")) {
        
        fd_ = open(port.c_str(), O_RDWR | O_NOCTTY);
        
        if (fd_ < 0) {
            RCLCPP_ERROR(logger_, "Failed to open serial port: %s", port.c_str());
            return;
        }
        
        struct termios tty;
        if (tcgetattr(fd_, &tty) != 0) {
            RCLCPP_ERROR(logger_, "Error getting serial attributes");
            close(fd_);
            fd_ = -1;
            return;
        }
        
        speed_t baud = B9600;
        if (baudrate == 115200) baud = B115200;
        else if (baudrate == 57600) baud = B57600;
        else if (baudrate == 38400) baud = B38400;
        else if (baudrate == 19200) baud = B19200;
        
        cfsetospeed(&tty, baud);
        cfsetispeed(&tty, baud);
        
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag |= CREAD | CLOCAL;
        
        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO;
        tty.c_lflag &= ~ECHOE;
        tty.c_lflag &= ~ECHONL;
        tty.c_lflag &= ~ISIG;
        
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
        
        tty.c_oflag &= ~OPOST;
        tty.c_oflag &= ~ONLCR;
        
        tty.c_cc[VTIME] = 10;
        tty.c_cc[VMIN] = 0;
        
        if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
            RCLCPP_ERROR(logger_, "Error setting serial attributes");
            close(fd_);
            fd_ = -1;
            return;
        }
        
        sleep(2);
        RCLCPP_INFO(logger_, "Connected to %s at %d baud", port.c_str(), baudrate);
    }
    
    ~SerialPort() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }
    
    bool isOpen() const {
        return fd_ >= 0;
    }
    
    bool write(const std::string& data) {
        if (fd_ < 0) return false;
        
        ssize_t bytes_written = ::write(fd_, data.c_str(), data.length());
        tcdrain(fd_);
        
        return bytes_written == static_cast<ssize_t>(data.length());
    }
};

// =======================
//  ESP32 Controller Node
// =======================
class Esp32Controller : public rclcpp::Node
{
public:
    Esp32Controller()
    : Node("esp32_controller",
           rclcpp::NodeOptions()
               .automatically_declare_parameters_from_overrides(true)
               .append_parameter_override("use_sim_time", true)),
      odom_received_(false),
      odom_x_min_(0.0),
      odom_x_max_(0.0),
      odom_y_min_(0.0),
      odom_y_max_(0.0),
      first_odom_(true),
      callback_counter_(0),
      current_roll_(0.0),
      current_pitch_(0.0),
      current_yaw_(0.0)
    {
        RCLCPP_INFO(get_logger(), "Starting esp32_controller node");

        current_odom_x_ = 0.0;
        current_odom_y_ = 0.0;

        // Get ROS parameters
        if (!this->has_parameter("callback_skip_rate")) {
            this->declare_parameter("callback_skip_rate", 5);
        }
        if (!this->has_parameter("x_sensitivity")) {
            this->declare_parameter("x_sensitivity", 1.5);
        }
        if (!this->has_parameter("y_sensitivity")) {
            this->declare_parameter("y_sensitivity", 2.0);
        }
        if (!this->has_parameter("lock_x_axis")) {
            this->declare_parameter("lock_x_axis", false);
        }
        if (!this->has_parameter("lock_y_axis")) {
            this->declare_parameter("lock_y_axis", false);
        }
        if (!this->has_parameter("enable_wrist_control")) {
            this->declare_parameter("enable_wrist_control", true);
        }
        if (!this->has_parameter("wrist_sensitivity")) {
            this->declare_parameter("wrist_sensitivity", 1.0);
        }
        
        callback_skip_rate_ = this->get_parameter("callback_skip_rate").as_int();
        x_sensitivity_ = this->get_parameter("x_sensitivity").as_double();
        y_sensitivity_ = this->get_parameter("y_sensitivity").as_double();
        lock_x_axis_ = this->get_parameter("lock_x_axis").as_bool();
        lock_y_axis_ = this->get_parameter("lock_y_axis").as_bool();
        enable_wrist_control_ = this->get_parameter("enable_wrist_control").as_bool();
        wrist_sensitivity_ = this->get_parameter("wrist_sensitivity").as_double();
        
        RCLCPP_INFO(get_logger(), "Parameters:");
        RCLCPP_INFO(get_logger(), "  - callback_skip_rate: %d (send commands every %d callbacks)", 
                   callback_skip_rate_, callback_skip_rate_);
        RCLCPP_INFO(get_logger(), "  - x_sensitivity: %.2fx (higher = more responsive)", 
                   x_sensitivity_);
        RCLCPP_INFO(get_logger(), "  - y_sensitivity: %.2fx (higher = more responsive)", 
                   y_sensitivity_);
        RCLCPP_INFO(get_logger(), "  - lock_x_axis: %s", lock_x_axis_ ? "LOCKED" : "unlocked");
        RCLCPP_INFO(get_logger(), "  - lock_y_axis: %s", lock_y_axis_ ? "LOCKED" : "unlocked");
        RCLCPP_INFO(get_logger(), "  - enable_wrist_control: %s", enable_wrist_control_ ? "YES" : "NO");
        RCLCPP_INFO(get_logger(), "  - wrist_sensitivity: %.2fx", wrist_sensitivity_);

        // Subscribe to odometry topic (from OTOS sensor)
        odom_sub_ = create_subscription<nav_msgs::msg::Odometry>(
            "/odom", 10,
            std::bind(&Esp32Controller::odomCallback, this, std::placeholders::_1)
        );

        // Subscribe to IMU topic (from MPU6050 sensor)
        imu_sub_ = create_subscription<sensor_msgs::msg::Imu>(
            "/imu/data", 10,
            std::bind(&Esp32Controller::imuCallback, this, std::placeholders::_1)
        );

        // Subscribe to joint states
        joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10,
            std::bind(&Esp32Controller::jointStateCallback, this, std::placeholders::_1)
        );

        RCLCPP_INFO(get_logger(), "Waiting for odometry data from /odom topic...");
        RCLCPP_INFO(get_logger(), "Move the OTOS sensor to calibrate min/max ranges");
    }

    void init()
    {
        RCLCPP_INFO(get_logger(), "Initializing MoveIt + Serial");

        // Enable serial port
        serial_ = std::make_unique<SerialPort>("/dev/ttyUSB0", 9600);
        if (!serial_->isOpen()) {
            RCLCPP_WARN(get_logger(), "Serial not available, running in dry mode");
        }

        move_group_ =
            std::make_shared<moveit::planning_interface::MoveGroupInterface>(
                shared_from_this(), "arm");

        robot_model_loader_ =
            std::make_shared<robot_model_loader::RobotModelLoader>(
                shared_from_this());

        robot_model_ = robot_model_loader_->getModel();
        if (!robot_model_) {
            RCLCPP_FATAL(get_logger(), "Failed to load robot model");
            rclcpp::shutdown();
            return;
        }

        robot_state_ =
            std::make_shared<moveit::core::RobotState>(robot_model_);
        robot_state_->setToDefaultValues();

        ee_link_ = move_group_->getEndEffectorLink();

        gd_ik_ = std::make_unique<GradientDescentIK>(
            robot_model_, "arm", ee_link_);

        RCLCPP_INFO(get_logger(), "Waiting for joint states...");
        rclcpp::Rate rate(10);
        int attempts = 0;
        while (rclcpp::ok() && attempts < 50) {
            try {
                auto joints = move_group_->getCurrentJointValues();
                if (!joints.empty()) {
                    RCLCPP_INFO(get_logger(), "Joint states received! (%zu joints)", joints.size());
                    break;
                }
            } catch (...) {
            }
            rclcpp::spin_some(shared_from_this());
            rate.sleep();
            attempts++;
        }

        if (attempts >= 50) {
            RCLCPP_WARN(get_logger(), 
                "Timed out waiting for joint states. Will continue anyway...");
        }

        RCLCPP_INFO(get_logger(), "esp32_controller ready");
    }

private:
    void jointStateCallback(const sensor_msgs::msg::JointState::SharedPtr msg)
    {
        std::lock_guard<std::mutex> lock(joint_state_mutex_);
        last_joint_state_ = msg;
    }

    void imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg)
    {
        // Extract quaternion
        double qx = msg->orientation.x;
        double qy = msg->orientation.y;
        double qz = msg->orientation.z;
        double qw = msg->orientation.w;
        
        // Convert quaternion to roll, pitch, yaw (Euler angles)
        // Roll (rotation around X-axis)
        double sinr_cosp = 2.0 * (qw * qx + qy * qz);
        double cosr_cosp = 1.0 - 2.0 * (qx * qx + qy * qy);
        current_roll_ = std::atan2(sinr_cosp, cosr_cosp);
        
        // Pitch (rotation around Y-axis)
        double sinp = 2.0 * (qw * qy - qz * qx);
        if (std::abs(sinp) >= 1)
            current_pitch_ = std::copysign(M_PI / 2, sinp);
        else
            current_pitch_ = std::asin(sinp);
        
        // Yaw (rotation around Z-axis)
        double siny_cosp = 2.0 * (qw * qz + qx * qy);
        double cosy_cosp = 1.0 - 2.0 * (qy * qy + qz * qz);
        current_yaw_ = std::atan2(siny_cosp, cosy_cosp);
        
        // Convert to degrees for logging
        double roll_deg = current_roll_ * 180.0 / M_PI;
        double pitch_deg = current_pitch_ * 180.0 / M_PI;
        double yaw_deg = current_yaw_ * 180.0 / M_PI;
        
        RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 1000,
            "IMU: Roll=%.1f° Pitch=%.1f° Yaw=%.1f°",
            roll_deg, pitch_deg, yaw_deg);
    }

    double mapRange(double value, double in_min, double in_max, double out_min, double out_max)
    {
        // Clamp input to range
        value = std::clamp(value, in_min, in_max);
        
        // Map to output range
        return out_min + (value - in_min) * (out_max - out_min) / (in_max - in_min);
    }

    void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        if (!gd_ik_) {
            return;
        }

        // ===============================
        // 1. Read Odometry Data (X, Y coordinates from OTOS sensor)
        // ===============================
        current_odom_x_ = msg->pose.pose.position.x;  // meters
        current_odom_y_ = msg->pose.pose.position.y;  // meters
        
        // Track min/max for calibration
        if (first_odom_) {
            odom_x_min_ = odom_x_max_ = current_odom_x_;
            odom_y_min_ = odom_y_max_ = current_odom_y_;
            first_odom_ = false;
            odom_received_ = true;
            RCLCPP_INFO(get_logger(), "First odometry data received!");
        } else {
            odom_x_min_ = std::min(odom_x_min_, current_odom_x_);
            odom_x_max_ = std::max(odom_x_max_, current_odom_x_);
            odom_y_min_ = std::min(odom_y_min_, current_odom_y_);
            odom_y_max_ = std::max(odom_y_max_, current_odom_y_);
        }

        // ===============================
        // 2. Get current end effector position
        // ===============================
        std::vector<double> joints;
        
        try {
            joints = move_group_->getCurrentJointValues();
        } catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(joint_state_mutex_);
            if (last_joint_state_) {
                joints = last_joint_state_->position;
            } else {
                return;
            }
        }

        const auto* jmg = robot_model_->getJointModelGroup("arm");

        if (joints.empty() || joints.size() != jmg->getVariableCount()) {
            return;
        }

        robot_state_->setJointGroupPositions(jmg, joints);
        robot_state_->update();

        const Eigen::Isometry3d& ee_pose = 
            robot_state_->getGlobalLinkTransform(ee_link_);
        
        Eigen::Vector3d ee_position = ee_pose.translation();

        // ===============================
        // 3. Scale odometry to robot workspace
        // ===============================
        // Target workspace bounds
        const double target_x_min = 0.6;
        const double target_x_max = 0.77;
        const double target_y_min = -1.5;
        const double target_y_max = -1.2;
        
        // Calculate center of the calibrated range
        double odom_x_center = (odom_x_min_ + odom_x_max_) / 2.0;
        double odom_y_center = (odom_y_min_ + odom_y_max_) / 2.0;
        
        // Calculate range with configurable sensitivities
        double odom_x_range = (odom_x_max_ - odom_x_min_) / x_sensitivity_;
        double odom_y_range = (odom_y_max_ - odom_y_min_) / y_sensitivity_;
        
        // New input bounds (centered)
        double odom_x_min_scaled = odom_x_center - odom_x_range;
        double odom_x_max_scaled = odom_x_center + odom_x_range;
        double odom_y_min_scaled = odom_y_center - odom_y_range;
        double odom_y_max_scaled = odom_y_center + odom_y_range;
        
        // Map to robot workspace
        double scaled_x = mapRange(current_odom_x_, odom_x_min_scaled, odom_x_max_scaled, 
                                target_x_min, target_x_max);
        double scaled_y = mapRange(current_odom_y_, odom_y_min_scaled, odom_y_max_scaled, 
                                target_y_min, target_y_max);
        
        // Apply hard bounds (safety)
        scaled_x = std::clamp(scaled_x, target_x_min, target_x_max);
        scaled_y = std::clamp(scaled_y, target_y_min, target_y_max);

        // ===============================
        // 4. Print OTOS Scaled coordinates only
        // ===============================
        RCLCPP_INFO(get_logger(), 
            "OTOS Scaled: [X: %.4f, Y: %.4f]",
            scaled_x, scaled_y);
        
        // ===============================
        // 5. Robot Movement Control (X and Y axes) with Callback-based Rate Limiting
        // ===============================
        // Target position: Use scaled OTOS X and Y (with axis locking), keep Z fixed
        double target_x = lock_x_axis_ ? ee_position.x() : scaled_x;  // Lock X if enabled
        double target_y = lock_y_axis_ ? ee_position.y() : scaled_y;  // Lock Y if enabled
        double target_z = 1.1;  
        
        Eigen::Vector3d target_position(target_x, target_y, target_z);
        
        // Increment callback counter
        callback_counter_++;
        
        // Check if we've reached the callback skip rate
        if (callback_counter_ < callback_skip_rate_) {
            return;  // Skip this update
        }
        
        // Reset counter since we're sending a command
        callback_counter_ = 0;
        
        // Solve IK for new target position
        if (gd_ik_->solveIK(target_position, joints)) {
            // Send serial commands to robot (servo0, servo1, servo2...)
            for (size_t i = 0; i < joints.size() - 1; i++) {
                int angle = static_cast<int>(std::round(joints[i] * 180.0 / M_PI));
                std::string cmd = "servo" + std::to_string(i) + "=" + std::to_string(angle) + "\n";
                
                if (serial_ && serial_->isOpen()) {
                    serial_->write(cmd);
                }
            }
            
            // ===============================
            // 6. Wrist Control (servo3) based on IMU Roll (reversed direction)
            // ===============================
            if (enable_wrist_control_ && serial_ && serial_->isOpen()) {
                // Convert roll from radians to degrees
                double roll_deg = current_roll_ * 180.0 / M_PI;
                
                // Apply sensitivity and map to servo range (0-180 degrees)
                // REVERSED: Subtract roll instead of add to reverse direction
                double wrist_angle = 90.0 - (roll_deg * wrist_sensitivity_);
                
                // Clamp to valid servo range
                wrist_angle = std::clamp(wrist_angle, 0.0, 180.0);
                
                int wrist_angle_int = static_cast<int>(std::round(wrist_angle));
                std::string wrist_cmd = "servo3=" + std::to_string(wrist_angle_int) + "\n";
                
                serial_->write(wrist_cmd);
                
                RCLCPP_INFO(get_logger(), "Wrist (servo3): %d° (IMU Roll: %.1f°)", 
                           wrist_angle_int, roll_deg);
            }
        }
    }

    // ---------- ROS ----------
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;

    // ---------- MoveIt ----------
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
    robot_model_loader::RobotModelLoaderPtr robot_model_loader_;
    moveit::core::RobotModelPtr robot_model_;
    moveit::core::RobotStatePtr robot_state_;
    std::string ee_link_;

    // ---------- Control ----------
    std::unique_ptr<GradientDescentIK> gd_ik_;
    std::unique_ptr<SerialPort> serial_;

    // ---------- Joint State Cache ----------
    sensor_msgs::msg::JointState::SharedPtr last_joint_state_;
    std::mutex joint_state_mutex_;

    // ---------- Odometry Data ----------
    double current_odom_x_;
    double current_odom_y_;
    bool odom_received_;
    
    // ---------- Calibration ----------
    double odom_x_min_, odom_x_max_;
    double odom_y_min_, odom_y_max_;
    bool first_odom_;
    
    // ---------- IMU Data ----------
    double current_roll_;
    double current_pitch_;
    double current_yaw_;
    
    // ---------- Callback-based Rate Limiting ----------
    int callback_counter_;
    int callback_skip_rate_;
    double x_sensitivity_;
    double y_sensitivity_;
    bool lock_x_axis_;
    bool lock_y_axis_;
    bool enable_wrist_control_;
    double wrist_sensitivity_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<Esp32Controller>();
    node->init();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}