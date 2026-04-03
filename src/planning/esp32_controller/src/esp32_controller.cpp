#include <memory>
#include <mutex>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <std_msgs/msg/string.hpp>
#include <geometry_msgs/msg/point.hpp>

#include <moveit/robot_model_loader/robot_model_loader.hpp>
#include <moveit/robot_state/robot_state.hpp>

#include "move_program/gradient_ik.hpp"

// ESP32 Controller Node
//
// Sim mode (use_trajectory_controller=true):
//   Subscribes to /odom, maps OTOS position to workspace coordinates,
//   publishes geometry_msgs/Point to /move_to. The move_server node
//   (launched by mujoco.launch.py) handles IK + MoveIt planning + execution.
//
// Real mode (use_trajectory_controller=false):
//   Full pipeline: /odom + /imu/data + /joint_states → IK → servo commands
//   published to /arm/servo_commands for pose_printer.
class Esp32Controller : public rclcpp::Node
{
public:
    Esp32Controller()
    : Node("esp32_controller",
           rclcpp::NodeOptions()
               .automatically_declare_parameters_from_overrides(true)),
      first_odom_(true),
      odom_x_min_(0.0), odom_x_max_(0.0),
      odom_y_min_(0.0), odom_y_max_(0.0),
      current_odom_x_(0.0), current_odom_y_(0.0),
      current_roll_(0.0), current_pitch_(0.0), current_yaw_(0.0),
      callback_counter_(0)
    {
        RCLCPP_INFO(get_logger(), "Starting esp32_controller node");

        // Declare parameters
        if (!has_parameter("callback_skip_rate"))
            declare_parameter("callback_skip_rate", 5);
        if (!has_parameter("x_sensitivity"))
            declare_parameter("x_sensitivity", 1.5);
        if (!has_parameter("y_sensitivity"))
            declare_parameter("y_sensitivity", 2.0);
        if (!has_parameter("lock_x_axis"))
            declare_parameter("lock_x_axis", false);
        if (!has_parameter("lock_y_axis"))
            declare_parameter("lock_y_axis", false);
        if (!has_parameter("lock_wrist"))
            declare_parameter("lock_wrist", false);
        if (!has_parameter("wrist_sensitivity"))
            declare_parameter("wrist_sensitivity", 1.0);
        if (!has_parameter("default_y_position"))
            declare_parameter("default_y_position", -0.03);
        if (!has_parameter("default_wrist_angle"))
            declare_parameter("default_wrist_angle", 90);
        if (!has_parameter("default_z_position"))
            declare_parameter("default_z_position", 0.19);
        if (!has_parameter("use_trajectory_controller"))
            declare_parameter("use_trajectory_controller", false);

        callback_skip_rate_ = get_parameter("callback_skip_rate").as_int();
        x_sensitivity_ = get_parameter("x_sensitivity").as_double();
        y_sensitivity_ = get_parameter("y_sensitivity").as_double();
        lock_x_axis_ = get_parameter("lock_x_axis").as_bool();
        lock_y_axis_ = get_parameter("lock_y_axis").as_bool();
        lock_wrist_ = get_parameter("lock_wrist").as_bool();
        wrist_sensitivity_ = get_parameter("wrist_sensitivity").as_double();
        default_y_position_ = get_parameter("default_y_position").as_double();
        default_wrist_angle_ = get_parameter("default_wrist_angle").as_int();
        default_z_position_ = get_parameter("default_z_position").as_double();
        sim_mode_ = get_parameter("use_trajectory_controller").as_bool();

        RCLCPP_INFO(get_logger(), "Mode: %s", sim_mode_ ? "SIM (publishing to /move_to)" : "REAL (IK → servo commands)");
        RCLCPP_INFO(get_logger(), "  x_sensitivity: %.2fx, y_sensitivity: %.2fx", x_sensitivity_, y_sensitivity_);
        RCLCPP_INFO(get_logger(), "  default_z_position: %.4f m", default_z_position_);

        // Subscribe to OTOS odometry (both modes)
        odom_sub_ = create_subscription<nav_msgs::msg::Odometry>(
            "/odom", 10,
            std::bind(&Esp32Controller::odomCallback, this, std::placeholders::_1));

        // Subscribe to IMU (both modes, used for wrist in real mode)
        imu_sub_ = create_subscription<sensor_msgs::msg::Imu>(
            "/imu/data", 10,
            std::bind(&Esp32Controller::imuCallback, this, std::placeholders::_1));

        if (sim_mode_) {
            // Sim mode: publish target XYZ to /move_to (consumed by move_server)
            move_to_pub_ = create_publisher<geometry_msgs::msg::Point>("/move_to", 10);
        } else {
            // Real mode: publish servo commands to pose_printer
            cmd_pub_ = create_publisher<std_msgs::msg::String>("/arm/servo_commands", 10);
            // Need joint states for IK seed in real mode
            joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
                "/joint_states", 10,
                std::bind(&Esp32Controller::jointStateCallback, this, std::placeholders::_1));
        }

        RCLCPP_INFO(get_logger(), "Waiting for odometry data from /odom topic...");
    }

    // Initialize MoveIt robot model (only needed for real mode IK)
    void init()
    {
        if (sim_mode_) {
            RCLCPP_INFO(get_logger(), "esp32_controller ready (sim mode — no IK needed)");
            initialized_ = true;
            return;
        }

        RCLCPP_INFO(get_logger(), "Initializing robot model for IK...");

        robot_model_loader_ =
            std::make_shared<robot_model_loader::RobotModelLoader>(shared_from_this());

        robot_model_ = robot_model_loader_->getModel();
        if (!robot_model_) {
            RCLCPP_FATAL(get_logger(), "Failed to load robot model");
            rclcpp::shutdown();
            return;
        }

        robot_state_ = std::make_shared<moveit::core::RobotState>(robot_model_);
        robot_state_->setToDefaultValues();

        // Get end effector link from SRDF
        for (const auto* ee_jmg : robot_model_->getEndEffectors()) {
            auto parent = ee_jmg->getEndEffectorParentGroup();
            if (parent.first == "arm") {
                ee_link_ = parent.second;
                break;
            }
        }
        if (ee_link_.empty()) {
            auto link_names = robot_model_->getJointModelGroup("arm")->getLinkModelNames();
            ee_link_ = link_names.back();
        }
        RCLCPP_INFO(get_logger(), "End effector link: %s", ee_link_.c_str());

        gd_ik_ = std::make_unique<GradientDescentIK>(robot_model_, "arm", ee_link_);

        initialized_ = true;
        RCLCPP_INFO(get_logger(), "esp32_controller ready (real mode — IK active)");
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
        current_pitch_ = std::abs(sinp) >= 1
            ? std::copysign(M_PI / 2, sinp)
            : std::asin(sinp);

        double siny_cosp = 2.0 * (qw * qz + qx * qy);
        double cosy_cosp = 1.0 - 2.0 * (qy * qy + qz * qz);
        current_yaw_ = std::atan2(siny_cosp, cosy_cosp);
    }

    double mapRange(double value, double in_min, double in_max, double out_min, double out_max)
    {
        value = std::clamp(value, in_min, in_max);
        return out_min + (value - in_min) * (out_max - out_min) / (in_max - in_min);
    }

    // Compute workspace target XYZ from OTOS odometry
    // Workspace bounds match the arm's reachable space (metres, base-frame)
    bool computeTarget(double& target_x, double& target_y, double& target_z)
    {
        const double target_x_min = -0.08;
        const double target_x_max = 0.09;
        const double target_y_min = 0.05;
        const double target_y_max = 0.20;

        double odom_x_center = (odom_x_min_ + odom_x_max_) / 2.0;
        double odom_y_center = (odom_y_min_ + odom_y_max_) / 2.0;
        double odom_x_range = (odom_x_max_ - odom_x_min_) / x_sensitivity_;
        double odom_y_range = (odom_y_max_ - odom_y_min_) / y_sensitivity_;

        double scaled_x, scaled_y;
        if (odom_x_range > 1e-6) {
            scaled_x = mapRange(current_odom_x_,
                odom_x_center - odom_x_range, odom_x_center + odom_x_range,
                target_x_min, target_x_max);
        } else {
            scaled_x = (target_x_min + target_x_max) / 2.0;
        }
        if (odom_y_range > 1e-6) {
            scaled_y = mapRange(current_odom_y_,
                odom_y_center - odom_y_range, odom_y_center + odom_y_range,
                target_y_min, target_y_max);
        } else {
            scaled_y = (target_y_min + target_y_max) / 2.0;
        }

        target_x = lock_x_axis_ ? (target_x_min + target_x_max) / 2.0
                                : std::clamp(scaled_x, target_x_min, target_x_max);
        target_y = lock_y_axis_ ? default_y_position_
                                : std::clamp(scaled_y, target_y_min, target_y_max);
        target_z = default_z_position_;
        return true;
    }

    void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        if (!initialized_) return;

        current_odom_x_ = msg->pose.pose.position.x;
        current_odom_y_ = msg->pose.pose.position.y;

        if (first_odom_) {
            odom_x_min_ = odom_x_max_ = current_odom_x_;
            odom_y_min_ = odom_y_max_ = current_odom_y_;
            first_odom_ = false;
            RCLCPP_INFO(get_logger(), "First odometry received — tracking OTOS range");
        } else {
            odom_x_min_ = std::min(odom_x_min_, current_odom_x_);
            odom_x_max_ = std::max(odom_x_max_, current_odom_x_);
            odom_y_min_ = std::min(odom_y_min_, current_odom_y_);
            odom_y_max_ = std::max(odom_y_max_, current_odom_y_);
        }

        // Rate-limit: only process every N callbacks
        callback_counter_++;
        if (callback_counter_ < callback_skip_rate_) return;
        callback_counter_ = 0;

        double target_x, target_y, target_z;
        if (!computeTarget(target_x, target_y, target_z)) return;

        RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 500,
            "Target: [X=%.4f, Y=%.4f, Z=%.4f]", target_x, target_y, target_z);

        if (sim_mode_) {
            // Sim mode: publish target point for move_server to handle
            geometry_msgs::msg::Point point;
            point.x = target_x;
            point.y = target_y;
            point.z = target_z;
            move_to_pub_->publish(point);
        } else {
            // Real mode: solve IK and publish servo commands
            realModePublish(target_x, target_y, target_z);
        }
    }

    void realModePublish(double target_x, double target_y, double target_z)
    {
        if (!gd_ik_) return;

        std::vector<double> joints;
        {
            std::lock_guard<std::mutex> lock(joint_state_mutex_);
            if (last_joint_state_) {
                joints = last_joint_state_->position;
            } else {
                RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 2000,
                    "No joint states received yet");
                return;
            }
        }

        const auto* jmg = robot_model_->getJointModelGroup("arm");
        if (joints.empty() || joints.size() != jmg->getVariableCount()) return;

        robot_state_->setJointGroupPositions(jmg, joints);
        robot_state_->update();

        Eigen::Vector3d target_position(target_x, target_y, target_z);

        if (gd_ik_->solveIK(target_position, joints)) {
            // Compute wrist angle
            double wrist_rad;
            if (lock_wrist_) {
                wrist_rad = static_cast<double>(default_wrist_angle_) * M_PI / 180.0;
            } else {
                double roll_deg = current_roll_ * 180.0 / M_PI;
                double wrist_deg = std::clamp(90.0 - (roll_deg * wrist_sensitivity_), 0.0, 180.0);
                wrist_rad = wrist_deg * M_PI / 180.0;
            }

            std::string commands;
            for (size_t i = 0; i < joints.size() - 1; i++) {
                int angle = static_cast<int>(std::round(joints[i] * 180.0 / M_PI));
                commands += "servo" + std::to_string(i) + "=" + std::to_string(angle) + "\n";
            }
            int wrist_angle_int = static_cast<int>(std::round(wrist_rad * 180.0 / M_PI));
            commands += "servo3=" + std::to_string(wrist_angle_int) + "\n";

            std_msgs::msg::String msg;
            msg.data = commands;
            cmd_pub_->publish(msg);
        }
    }

    // Mode
    bool sim_mode_;
    bool initialized_ = false;

    // Publishers
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr move_to_pub_;  // sim mode
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr cmd_pub_;          // real mode

    // Subscriptions
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;

    // MoveIt components (real mode only)
    robot_model_loader::RobotModelLoaderPtr robot_model_loader_;
    moveit::core::RobotModelPtr robot_model_;
    moveit::core::RobotStatePtr robot_state_;
    std::string ee_link_;
    std::unique_ptr<GradientDescentIK> gd_ik_;

    // Joint state cache (real mode only)
    sensor_msgs::msg::JointState::SharedPtr last_joint_state_;
    std::mutex joint_state_mutex_;

    // Odometry tracking
    bool first_odom_;
    double odom_x_min_, odom_x_max_;
    double odom_y_min_, odom_y_max_;
    double current_odom_x_, current_odom_y_;

    // IMU data
    double current_roll_, current_pitch_, current_yaw_;

    // Parameters
    int callback_counter_;
    int callback_skip_rate_;
    double x_sensitivity_, y_sensitivity_;
    bool lock_x_axis_, lock_y_axis_, lock_wrist_;
    double wrist_sensitivity_;
    double default_y_position_, default_z_position_;
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
