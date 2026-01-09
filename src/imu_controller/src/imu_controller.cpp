#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>

#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/robot_model_loader/robot_model_loader.hpp>
#include <moveit/robot_state/robot_state.hpp>

class ImuController : public rclcpp::Node
{
public:
    ImuController()
    : Node("imu_controller")
    {
        RCLCPP_INFO(get_logger(), "Starting imu_controller node");

        // IMU subscription (safe in constructor)
        imu_sub_ = create_subscription<sensor_msgs::msg::Imu>(
            "/imu/raw", 10,
            std::bind(&ImuController::imuCallback, this, std::placeholders::_1)
        );
    }

    // 🔑 Must be called AFTER node is in a shared_ptr
    void initMoveIt()
    {
        RCLCPP_INFO(get_logger(), "Initializing MoveIt interfaces");

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
        RCLCPP_INFO(get_logger(), "End effector link: %s", ee_link_.c_str());
    }

private:
    void imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg)
    {
        if (!move_group_) {
            RCLCPP_WARN_THROTTLE(
                get_logger(), *get_clock(), 2000,
                "MoveIt not initialized yet"
            );
            return;
        }

        // ---- IMU DATA ----
        RCLCPP_INFO_THROTTLE(
            get_logger(), *get_clock(), 1000,
            "IMU | gyro [%.3f %.3f %.3f] accel [%.3f %.3f %.3f]",
            msg->angular_velocity.x,
            msg->angular_velocity.y,
            msg->angular_velocity.z,
            msg->linear_acceleration.x,
            msg->linear_acceleration.y,
            msg->linear_acceleration.z
        );

        // ---- ROBOT STATE ----
        std::vector<double> joints = move_group_->getCurrentJointValues();
        robot_state_->setJointGroupPositions("arm", joints);

        const Eigen::Isometry3d& ee_pose =
            robot_state_->getGlobalLinkTransform(ee_link_);

        Eigen::Vector3d pos = ee_pose.translation();

        RCLCPP_INFO_THROTTLE(
            get_logger(), *get_clock(), 1000,
            "EE Pose | x=%.3f y=%.3f z=%.3f",
            pos.x(), pos.y(), pos.z()
        );
    }

    // ROS
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;

    // MoveIt
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
    robot_model_loader::RobotModelLoaderPtr robot_model_loader_;
    moveit::core::RobotModelPtr robot_model_;
    moveit::core::RobotStatePtr robot_state_;
    std::string ee_link_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    // ✅ Node now owned by shared_ptr
    auto node = std::make_shared<ImuController>();

    // ✅ Safe to call shared_from_this()
    node->initMoveIt();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

