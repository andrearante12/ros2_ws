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

#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/robot_model_loader/robot_model_loader.hpp>
#include <moveit/robot_state/robot_state.hpp>

// =======================
//  Your existing classes
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
        
        RCLCPP_INFO(logger_, "Starting IK solver for Target: [%.3f, %.3f, %.3f]", 
                    target_pos.x(), target_pos.y(), target_pos.z());
        
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
        
        // Set baud rate
        speed_t baud = B9600;
        if (baudrate == 115200) baud = B115200;
        else if (baudrate == 57600) baud = B57600;
        else if (baudrate == 38400) baud = B38400;
        else if (baudrate == 19200) baud = B19200;
        
        cfsetospeed(&tty, baud);
        cfsetispeed(&tty, baud);
        
        // 8N1 mode
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
        
        sleep(2); // Allow Arduino reset
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
        tcdrain(fd_); // Wait for data to be transmitted (like flush)
        
        return bytes_written == static_cast<ssize_t>(data.length());
    }
};

class Esp32Controller : public rclcpp::Node
{
public:
    Esp32Controller()
    : Node("esp32_controller",
           rclcpp::NodeOptions()
               .automatically_declare_parameters_from_overrides(true)
               .append_parameter_override("use_sim_time", true)),
      is_calibrated_(false),
      calibration_samples_(0),
      calibration_sample_count_(50),  // Number of samples for calibration
      is_moving_(false),
      callbacks_since_last_command_(1000)  // Start high so first command isn't blocked
    {
        RCLCPP_INFO(get_logger(), "Starting esp32_controller node");

        // Initialize bias to zero
        gyro_bias_ = Eigen::Vector3d::Zero();
        accel_bias_ = Eigen::Vector3d::Zero();
        last_target_position_ = Eigen::Vector3d::Zero();

        // Open movement log file
        movement_log_.open("movement_log.txt", std::ios::out);
        if (movement_log_.is_open()) {
            movement_log_ << "=== ESP32 Controller Movement Log ===\n";
            movement_log_ << "Started at: " << this->now().seconds() << "\n\n";
            RCLCPP_INFO(get_logger(), "Movement log file opened: movement_log.txt");
        } else {
            RCLCPP_ERROR(get_logger(), "Failed to open movement log file");
        }

        // Subscribe to IMU control topic
        imu_sub_ = create_subscription<sensor_msgs::msg::Imu>(
            "/imu/raw", 10,
            std::bind(&Esp32Controller::imuCallback, this, std::placeholders::_1)
        );

        // Subscribe to joint states as backup
        joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10,
            std::bind(&Esp32Controller::jointStateCallback, this, std::placeholders::_1)
        );

        RCLCPP_INFO(get_logger(), "Calibrating IMU... Keep sensor stationary for %d samples", 
                    calibration_sample_count_);
    }

    ~Esp32Controller() {
        if (movement_log_.is_open()) {
            movement_log_.close();
            RCLCPP_INFO(get_logger(), "Movement log file closed");
        }
    }

    // Must be called AFTER node is owned by shared_ptr
    void init()
    {
        RCLCPP_INFO(get_logger(), "Initializing MoveIt + Serial");

        // ---- Serial (you can change params later) ----
        serial_ = std::make_unique<SerialPort>("/dev/ttyUSB0", 9600);
        if (!serial_->isOpen()) {
            RCLCPP_WARN(get_logger(), "Serial not available, running in dry mode");
        }

        // ---- MoveIt ----
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

        // Wait for joint states to be available
        RCLCPP_INFO(get_logger(), "Waiting for joint states...");
        rclcpp::Rate rate(10); // 10 Hz
        int attempts = 0;
        while (rclcpp::ok() && attempts < 50) {
            try {
                auto joints = move_group_->getCurrentJointValues();
                if (!joints.empty()) {
                    RCLCPP_INFO(get_logger(), "Joint states received! (%zu joints)", joints.size());
                    break;
                }
            } catch (...) {
                // Ignore exceptions during initialization
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
        // Store the latest joint states
        std::lock_guard<std::mutex> lock(joint_state_mutex_);
        last_joint_state_ = msg;
    }

    void imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg)
    {
        if (!gd_ik_) {
            RCLCPP_WARN_THROTTLE(
                get_logger(), *get_clock(), 2000,
                "Controller not initialized yet"
            );
            return;
        }

        // ===============================
        // 0. Calibration Phase
        // ===============================
        if (!is_calibrated_) {
            // Accumulate samples for bias calculation
            gyro_bias_ += Eigen::Vector3d(
                msg->angular_velocity.x,
                msg->angular_velocity.y,
                msg->angular_velocity.z
            );
            
            accel_bias_ += Eigen::Vector3d(
                msg->linear_acceleration.x,
                msg->linear_acceleration.y,
                msg->linear_acceleration.z - 9.81  // Subtract gravity from Z axis
            );
            
            calibration_samples_++;
            
            if (calibration_samples_ >= calibration_sample_count_) {
                // Calculate average bias
                gyro_bias_ /= calibration_sample_count_;
                accel_bias_ /= calibration_sample_count_;
                
                is_calibrated_ = true;
                
                RCLCPP_INFO(get_logger(), "IMU Calibration Complete!");
                RCLCPP_INFO(get_logger(), "Gyro Bias: [%.4f, %.4f, %.4f] rad/s",
                           gyro_bias_.x(), gyro_bias_.y(), gyro_bias_.z());
                RCLCPP_INFO(get_logger(), "Accel Bias: [%.4f, %.4f, %.4f] m/s²",
                           accel_bias_.x(), accel_bias_.y(), accel_bias_.z());
                RCLCPP_INFO(get_logger(), "Now publishing calibrated IMU data...");
            } else {
                RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 1000,
                                    "Calibrating... %d/%d samples",
                                    calibration_samples_, calibration_sample_count_);
            }
            return;
        }

        // ===============================
        // 1. Read and Calibrate IMU (INPUT)
        // ===============================
        // Apply bias correction
        double gx = msg->angular_velocity.x - gyro_bias_.x();
        double gy = msg->angular_velocity.y - gyro_bias_.y();
        double gz = msg->angular_velocity.z - gyro_bias_.z();

        // Round to 2 decimal places
        auto round2 = [](double value) -> double {
            return std::round(value * 100.0) / 100.0;
        };

        double ax = round2(msg->linear_acceleration.x - accel_bias_.x());
        double ay = round2(msg->linear_acceleration.y - accel_bias_.y());
        double az = round2(msg->linear_acceleration.z - 9.81 - accel_bias_.z());  // Remove gravity

        // Apply deadband to filter out noise when stationary
        auto apply_deadband = [](double value, double threshold = 0.02) -> double {
            return (std::abs(value) < threshold) ? 0.0 : value;
        };

        ax = apply_deadband(ax);
        ay = apply_deadband(ay);
        az = apply_deadband(az);
        gx = apply_deadband(gx, 0.01);  // Smaller threshold for gyro
        gy = apply_deadband(gy, 0.01);
        gz = apply_deadband(gz, 0.01);

        // Print calibrated IMU values to terminal
        RCLCPP_INFO(get_logger(), 
            "IMU Data - Gyro: [%.3f, %.3f, %.3f] rad/s | Accel: [%.2f, %.2f, %.2f] m/s²",
            gx, gy, gz, ax, ay, az);

        // ===============================
        // 2. Get current joint state and compute end effector position
        // ===============================
        std::vector<double> joints;
        
        // Try MoveIt first
        try {
            joints = move_group_->getCurrentJointValues();
        } catch (const std::exception& e) {
            // Fall back to direct joint_states subscription
            std::lock_guard<std::mutex> lock(joint_state_mutex_);
            if (last_joint_state_) {
                RCLCPP_INFO_ONCE(get_logger(), 
                    "MoveIt getCurrentJointValues failed, using direct /joint_states subscription");
                joints = last_joint_state_->position;
            } else {
                RCLCPP_WARN_THROTTLE(
                    get_logger(), *get_clock(), 5000,
                    "No joint states available yet. Waiting for /joint_states messages..."
                );
                RCLCPP_INFO(get_logger(), "----------------------------------------");
                return;
            }
        }

        const auto* jmg = robot_model_->getJointModelGroup("arm");

        if (joints.empty() || joints.size() != jmg->getVariableCount()) {
            RCLCPP_WARN_THROTTLE(
                get_logger(), *get_clock(), 5000,
                "Invalid joint count. Expected %zu joints for 'arm' group, got %zu total joints",
                jmg->getVariableCount(), joints.size()
            );
            RCLCPP_INFO(get_logger(), "----------------------------------------");
            return;
        }

        // Update robot state with current joint values
        robot_state_->setJointGroupPositions(jmg, joints);
        robot_state_->update();

        // Get end effector pose
        const Eigen::Isometry3d& ee_pose = 
            robot_state_->getGlobalLinkTransform(ee_link_);
        
        Eigen::Vector3d ee_position = ee_pose.translation();

        // Print end effector XYZ coordinates to terminal
        RCLCPP_INFO(get_logger(), 
            "End Effector Position - X: %.4f, Y: %.4f, Z: %.4f",
            ee_position.x(), ee_position.y(), ee_position.z());

        // Calculate target position using IMU as joystick (proportional control)
        // Tilt the IMU to command end effector movement in that direction
        double sensitivity = 0.05;  // Adjust this: higher = more sensitive (meters per m/s²)
        
        double target_x = ee_position.x() + (ax * sensitivity); 
        double target_y = ee_position.y() + (ay * sensitivity); 
        double target_z = ee_position.z() + (az * sensitivity); 

        // Apply limits to target position
        double x_min = 0.6, x_max = 0.77;
        double y_min = -1.5, y_max = -1.2;
        
        target_x = std::clamp(target_x, x_min, x_max);
        target_y = std::clamp(target_y, y_min, y_max);
        
        Eigen::Vector3d target_position(target_x, target_y, ee_position.z()); // NOTE: currently fixing the z-axis for testing purposes
        
        // Calculate change in each axis
        double delta_x = std::abs(target_x - ee_position.x());
        double delta_y = std::abs(target_y - ee_position.y());
        double delta_z = std::abs(target_z - ee_position.z());
        
        RCLCPP_INFO(get_logger(), 
            "Target Position - X: %.4f, Y: %.4f, Z: %.4f (ΔX: %.4f, ΔY: %.4f, ΔZ: %.4f)", 
            target_x, target_y, target_z,
            delta_x, delta_y, delta_z);

        // Only send movement command if X or Y change is significant
        double movement_threshold = 0.01;  // meters
        int callback_rate_limit = 2;  // Number of callbacks to skip between commands (increased to reduce rapid commands)
        
        // Increment callback counter
        callbacks_since_last_command_++;
        
        if (delta_x > movement_threshold || delta_y > movement_threshold) {
            RCLCPP_INFO(get_logger(), "Movement threshold exceeded!");
            
            // Check if enough callbacks have passed since last command
            if (callbacks_since_last_command_ < callback_rate_limit) {
                RCLCPP_INFO(get_logger(), 
                    "Command rate limited. %d more callbacks needed.",
                    callback_rate_limit - callbacks_since_last_command_);
                RCLCPP_INFO(get_logger(), "----------------------------------------");
                return;
            }
            
            // Check if we're already executing a movement
            if (is_moving_) {
                RCLCPP_INFO(get_logger(), "Robot still moving to previous target. Ignoring new command.");
                RCLCPP_INFO(get_logger(), "----------------------------------------");
                return;
            }
            
            // Additional check: Ignore if acceleration is too high (transient movement)
            double accel_magnitude = std::sqrt(ax*ax + ay*ay + az*az);
            if (accel_magnitude > 2.0) {  // Threshold for "actively moving IMU"
                RCLCPP_INFO(get_logger(), 
                    "IMU acceleration too high (%.2f m/s²) - likely transient movement. Ignoring.",
                    accel_magnitude);
                RCLCPP_INFO(get_logger(), "----------------------------------------");
                return;
            }
            
            RCLCPP_INFO(get_logger(), "Significant movement detected (ΔX: %.4f, ΔY: %.4f) - Sending IK command...", delta_x, delta_y);
            
            is_moving_ = true;
            callbacks_since_last_command_ = 0;  // Reset counter
            last_target_position_ = target_position;
            
            RCLCPP_INFO(get_logger(), "Solving IK for target: [%.4f, %.4f, %.4f]", 
                       target_position.x(), target_position.y(), target_position.z());
            
            if (gd_ik_->solveIK(target_position, joints)) {
                RCLCPP_INFO(get_logger(), "IK solution found! Joint values:");
                
                // Log successful movement to text file
                if (movement_log_.is_open()) {
                    auto now = this->now();
                    movement_log_ << "[TIMESTAMP: " << std::fixed << std::setprecision(6) 
                                 << now.seconds() << "]\n";
                    movement_log_ << "IMU Data - Gyro: [" << std::setprecision(3) 
                                 << gx << ", " << gy << ", " << gz << "] rad/s | Accel: ["
                                 << std::setprecision(2) << ax << ", " << ay << ", " << az << "] m/s²\n";
                    movement_log_ << "End Effector Position - X: " << std::setprecision(4) 
                                 << ee_position.x() << ", Y: " << ee_position.y() 
                                 << ", Z: " << ee_position.z() << "\n";
                    movement_log_ << "Target Position - X: " << target_position.x() 
                                 << ", Y: " << target_position.y() << ", Z: " << target_position.z()
                                 << " (ΔX: " << delta_x << ", ΔY: " << delta_y 
                                 << ", ΔZ: " << delta_z << ")\n";
                    movement_log_ << "Joint Values: [";
                    for (size_t i = 0; i < std::min(joints.size(), size_t(4)); i++) {
                        if (i > 0) movement_log_ << ", ";
                        movement_log_ << std::setprecision(6) << joints[i];
                    }
                    movement_log_ << "]\n";
                    movement_log_ << "----------------------------------------\n\n";
                    movement_log_.flush();
                }
                
                // Send serial commands
                for (size_t i = 0; i < joints.size() - 1; i++) {
                    int angle = static_cast<int>(std::round(joints[i] * 180.0 / M_PI));
                    std::string cmd = "servo" + std::to_string(i) + "=" + std::to_string(angle) + "\n";
                    
                    if (serial_->isOpen()) {
                        serial_->write(cmd);
                    }
                }
                
                // Reset moving flag after a delay (adjust based on your robot's speed)
                // For now, the rate limiter handles this
                is_moving_ = false;
                
            } else {
                RCLCPP_WARN(get_logger(), "IK solver failed for target position");
                is_moving_ = false;
            }
            
        } else {
            RCLCPP_DEBUG(get_logger(), "Movement too small (ΔX: %.4f, ΔY: %.4f < %.4f m) - Ignoring", delta_x, delta_y, movement_threshold);
        }

        // Add separator for readability
        RCLCPP_INFO(get_logger(), "----------------------------------------");

    }

    // ---------- ROS ----------
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

    // ---------- IMU Calibration ----------
    bool is_calibrated_;
    int calibration_samples_;
    int calibration_sample_count_;
    Eigen::Vector3d gyro_bias_;
    Eigen::Vector3d accel_bias_;

    // ---------- Movement Control ----------
    bool is_moving_;
    int callbacks_since_last_command_;
    Eigen::Vector3d last_target_position_;
    
    // ---------- Movement Logging ----------
    std::ofstream movement_log_;
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