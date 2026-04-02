#include <memory>
#include <mutex>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <std_msgs/msg/string.hpp>

#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/robot_model_loader/robot_model_loader.hpp>
#include <moveit/robot_state/robot_state.hpp>

#include "move_program/gradient_ik.hpp"

// ESP32 Controller Node
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

        // Declare and get ROS parameters
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
        if (!this->has_parameter("lock_wrist")) {
            this->declare_parameter("lock_wrist", false);
        }
        if (!this->has_parameter("wrist_sensitivity")) {
            this->declare_parameter("wrist_sensitivity", 1.0);
        }
        if (!this->has_parameter("default_y_position")) {
            this->declare_parameter("default_y_position", -0.03);
        }
        if (!this->has_parameter("default_wrist_angle")) {
            this->declare_parameter("default_wrist_angle", 90);
        }
        if (!this->has_parameter("default_z_position")) {
            this->declare_parameter("default_z_position", 0.19);
        }

        callback_skip_rate_ = this->get_parameter("callback_skip_rate").as_int();
        x_sensitivity_ = this->get_parameter("x_sensitivity").as_double();
        y_sensitivity_ = this->get_parameter("y_sensitivity").as_double();
        lock_x_axis_ = this->get_parameter("lock_x_axis").as_bool();
        lock_y_axis_ = this->get_parameter("lock_y_axis").as_bool();
        lock_wrist_ = this->get_parameter("lock_wrist").as_bool();
        wrist_sensitivity_ = this->get_parameter("wrist_sensitivity").as_double();
        default_y_position_ = this->get_parameter("default_y_position").as_double();
        default_wrist_angle_ = this->get_parameter("default_wrist_angle").as_int();
        default_z_position_ = this->get_parameter("default_z_position").as_double();

        RCLCPP_INFO(get_logger(), "Parameters:");
        RCLCPP_INFO(get_logger(), "  - callback_skip_rate: %d", callback_skip_rate_);
        RCLCPP_INFO(get_logger(), "  - x_sensitivity: %.2fx", x_sensitivity_);
        RCLCPP_INFO(get_logger(), "  - y_sensitivity: %.2fx", y_sensitivity_);
        RCLCPP_INFO(get_logger(), "  - lock_x_axis: %s", lock_x_axis_ ? "LOCKED" : "unlocked");
        RCLCPP_INFO(get_logger(), "  - lock_y_axis: %s", lock_y_axis_ ? "LOCKED" : "unlocked");
        RCLCPP_INFO(get_logger(), "  - lock_wrist: %s", lock_wrist_ ? "LOCKED" : "unlocked");
        RCLCPP_INFO(get_logger(), "  - wrist_sensitivity: %.2fx", wrist_sensitivity_);
        RCLCPP_INFO(get_logger(), "  - default_y_position: %.4f m", default_y_position_);
        RCLCPP_INFO(get_logger(), "  - default_wrist_angle: %d°", default_wrist_angle_);
        RCLCPP_INFO(get_logger(), "  - default_z_position: %.4f m", default_z_position_);

        // Publisher — pose_printer owns the serial port
        cmd_pub_ = this->create_publisher<std_msgs::msg::String>("/arm/servo_commands", 10);

        // Subscribe to sensor topics
        odom_sub_ = create_subscription<nav_msgs::msg::Odometry>(
            "/odom", 10,
            std::bind(&Esp32Controller::odomCallback, this, std::placeholders::_1)
        );

        imu_sub_ = create_subscription<sensor_msgs::msg::Imu>(
            "/imu/data", 10,
            std::bind(&Esp32Controller::imuCallback, this, std::placeholders::_1)
        );

        joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10,
            std::bind(&Esp32Controller::jointStateCallback, this, std::placeholders::_1)
        );

        RCLCPP_INFO(get_logger(), "Waiting for odometry data from /odom topic...");
        RCLCPP_INFO(get_logger(), "Move the OTOS sensor to calibrate min/max ranges");
    }

    void init()
    {
        RCLCPP_INFO(get_logger(), "Initializing MoveIt");

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
        double qx = msg->orientation.x;
        double qy = msg->orientation.y;
        double qz = msg->orientation.z;
        double qw = msg->orientation.w;

        double sinr_cosp = 2.0 * (qw * qx + qy * qz);
        double cosr_cosp = 1.0 - 2.0 * (qx * qx + qy * qy);
        current_roll_ = std::atan2(sinr_cosp, cosr_cosp);

        double sinp = 2.0 * (qw * qy - qz * qx);
        if (std::abs(sinp) >= 1)
            current_pitch_ = std::copysign(M_PI / 2, sinp);
        else
            current_pitch_ = std::asin(sinp);

        double siny_cosp = 2.0 * (qw * qz + qx * qy);
        double cosy_cosp = 1.0 - 2.0 * (qy * qy + qz * qz);
        current_yaw_ = std::atan2(siny_cosp, cosy_cosp);

        RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 1000,
            "IMU: Roll=%.1f° Pitch=%.1f° Yaw=%.1f°",
            current_roll_ * 180.0 / M_PI,
            current_pitch_ * 180.0 / M_PI,
            current_yaw_ * 180.0 / M_PI);
    }

    double mapRange(double value, double in_min, double in_max, double out_min, double out_max)
    {
        value = std::clamp(value, in_min, in_max);
        return out_min + (value - in_min) * (out_max - out_min) / (in_max - in_min);
    }

    void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        if (!gd_ik_) {
            return;
        }

        current_odom_x_ = msg->pose.pose.position.x;
        current_odom_y_ = msg->pose.pose.position.y;

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

        const double target_x_min = 0.6;
        const double target_x_max = 0.77;
        const double target_y_min = -1.5;
        const double target_y_max = -1.2;

        double odom_x_center = (odom_x_min_ + odom_x_max_) / 2.0;
        double odom_y_center = (odom_y_min_ + odom_y_max_) / 2.0;

        double odom_x_range = (odom_x_max_ - odom_x_min_) / x_sensitivity_;
        double odom_y_range = (odom_y_max_ - odom_y_min_) / y_sensitivity_;

        double scaled_x = mapRange(current_odom_x_,
            odom_x_center - odom_x_range, odom_x_center + odom_x_range,
            target_x_min, target_x_max);
        double scaled_y = mapRange(current_odom_y_,
            odom_y_center - odom_y_range, odom_y_center + odom_y_range,
            target_y_min, target_y_max);

        scaled_x = std::clamp(scaled_x, target_x_min, target_x_max);
        scaled_y = std::clamp(scaled_y, target_y_min, target_y_max);

        double target_x = lock_x_axis_ ? ee_position.x() : scaled_x;
        double target_y = lock_y_axis_ ? default_y_position_ : scaled_y;
        double target_z = default_z_position_;

        RCLCPP_INFO(get_logger(), "Target: [X=%.4f, Y=%.4f, Z=%.4f]", target_x, target_y, target_z);

        Eigen::Vector3d target_position(target_x, target_y, target_z);

        callback_counter_++;
        if (callback_counter_ < callback_skip_rate_) {
            return;
        }
        callback_counter_ = 0;

        if (gd_ik_->solveIK(target_position, joints)) {
            std::string commands;

            for (size_t i = 0; i < joints.size() - 1; i++) {
                int angle = static_cast<int>(std::round(joints[i] * 180.0 / M_PI));
                commands += "servo" + std::to_string(i) + "=" + std::to_string(angle) + "\n";
            }

            // Wrist control
            int wrist_angle_int;
            if (lock_wrist_) {
                wrist_angle_int = default_wrist_angle_;
                RCLCPP_INFO(get_logger(), "Wrist (servo3): %d° (LOCKED)", wrist_angle_int);
            } else {
                double roll_deg = current_roll_ * 180.0 / M_PI;
                double wrist_angle = std::clamp(90.0 - (roll_deg * wrist_sensitivity_), 0.0, 180.0);
                wrist_angle_int = static_cast<int>(std::round(wrist_angle));
                RCLCPP_INFO(get_logger(), "Wrist (servo3): %d° (IMU Roll: %.1f°)",
                           wrist_angle_int, roll_deg);
            }
            commands += "servo3=" + std::to_string(wrist_angle_int) + "\n";

            std_msgs::msg::String msg;
            msg.data = commands;
            cmd_pub_->publish(msg);
        }
    }

    // Publishers
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr cmd_pub_;

    // Subscriptions
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;

    // MoveIt components
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
    robot_model_loader::RobotModelLoaderPtr robot_model_loader_;
    moveit::core::RobotModelPtr robot_model_;
    moveit::core::RobotStatePtr robot_state_;
    std::string ee_link_;

    // IK solver
    std::unique_ptr<GradientDescentIK> gd_ik_;

    // Joint state cache
    sensor_msgs::msg::JointState::SharedPtr last_joint_state_;
    std::mutex joint_state_mutex_;

    // Odometry data
    double current_odom_x_;
    double current_odom_y_;
    bool odom_received_;

    double odom_x_min_, odom_x_max_;
    double odom_y_min_, odom_y_max_;
    bool first_odom_;

    // IMU data
    double current_roll_;
    double current_pitch_;
    double current_yaw_;

    // Control parameters
    int callback_counter_;
    int callback_skip_rate_;
    double x_sensitivity_;
    double y_sensitivity_;
    bool lock_x_axis_;
    bool lock_y_axis_;
    bool lock_wrist_;
    double wrist_sensitivity_;
    double default_y_position_;
    double default_z_position_;
    int default_wrist_angle_;
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
