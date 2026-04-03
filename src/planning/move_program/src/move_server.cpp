#include <memory>
#include <mutex>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <control_msgs/action/follow_joint_trajectory.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>
#include <moveit/robot_model_loader/robot_model_loader.hpp>
#include <moveit/robot_state/robot_state.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/bool.hpp>

#include "move_program/gradient_ik.hpp"

class MoveServer : public rclcpp::Node
{
    using FollowJT = control_msgs::action::FollowJointTrajectory;
    using GoalHandle = rclcpp_action::ClientGoalHandle<FollowJT>;

public:
    MoveServer()
    : Node("move_server",
           rclcpp::NodeOptions()
               .automatically_declare_parameters_from_overrides(true)
               .append_parameter_override("use_sim_time", true))
    {
        result_pub_ = this->create_publisher<std_msgs::msg::Bool>("/move_to/result", 10);

        waypoint_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
            "/move_to", 10,
            std::bind(&MoveServer::waypoint_cb, this, std::placeholders::_1));

        joint_state_sub_ = this->create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10,
            [this](const sensor_msgs::msg::JointState::SharedPtr msg) {
                std::lock_guard<std::mutex> lock(js_mutex_);
                last_joint_state_ = msg;
            });

        // Action client — sends joint goals directly to controller (bypasses MoveIt planning)
        action_client_ = rclcpp_action::create_client<FollowJT>(
            this, "/robotic_arm_controller/follow_joint_trajectory");

        RCLCPP_INFO(this->get_logger(), "Waiting for robot model...");
    }

    void initialize()
    {
        robot_model_loader::RobotModelLoader loader(shared_from_this());
        robot_model_ = loader.getModel();
        if (!robot_model_) {
            RCLCPP_ERROR(this->get_logger(), "Could not load robot model!");
            return;
        }

        robot_state_ = std::make_shared<moveit::core::RobotState>(robot_model_);
        robot_state_->setToDefaultValues();

        // Get ee_link from SRDF (same pattern as esp32_controller)
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

        gd_ik_ = std::make_unique<GradientDescentIK>(robot_model_, "arm", ee_link_);

        initialized_ = true;
        RCLCPP_INFO(this->get_logger(), "Move server ready (direct controller mode) — publish to /move_to");
    }

private:
    // Workspace bounds
    static constexpr double WS_X_MIN = -0.08, WS_X_MAX = 0.09;
    static constexpr double WS_Y_MIN = -0.18, WS_Y_MAX = 0.18;
    static constexpr double WS_Z_MIN = 0.05,  WS_Z_MAX = 0.25;

    bool inWorkspace(double x, double y, double z)
    {
        return x >= WS_X_MIN && x <= WS_X_MAX &&
               y >= WS_Y_MIN && y <= WS_Y_MAX &&
               z >= WS_Z_MIN && z <= WS_Z_MAX;
    }

    std::vector<double> getArmJoints()
    {
        std::lock_guard<std::mutex> lock(js_mutex_);
        if (!last_joint_state_) return {};

        const auto* jmg = robot_model_->getJointModelGroup("arm");
        auto arm_joint_names = jmg->getVariableNames();
        std::vector<double> positions(arm_joint_names.size(), 0.0);

        for (size_t i = 0; i < arm_joint_names.size(); i++) {
            for (size_t j = 0; j < last_joint_state_->name.size(); j++) {
                if (last_joint_state_->name[j] == arm_joint_names[i]) {
                    positions[i] = last_joint_state_->position[j];
                    break;
                }
            }
        }
        return positions;
    }

    void waypoint_cb(const geometry_msgs::msg::Point::SharedPtr msg)
    {
        if (!initialized_) return;

        // Bounds check — reject instantly
        if (!inWorkspace(msg->x, msg->y, msg->z)) {
            RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 1000,
                "Target [%.3f, %.3f, %.3f] outside workspace", msg->x, msg->y, msg->z);
            publish_result(false);
            return;
        }

        // Check action server availability
        if (!action_client_->action_server_is_ready()) {
            RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 2000,
                "FollowJointTrajectory action server not ready");
            return;
        }

        Eigen::Vector3d target(msg->x, msg->y, msg->z);

        // Get current joints as IK seed
        std::vector<double> joint_solution = getArmJoints();
        if (joint_solution.empty()) {
            RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 2000,
                "No joint states yet");
            return;
        }

        // Solve IK (~50-150ms)
        if (!gd_ik_->solveIK(target, joint_solution)) {
            RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 1000,
                "IK failed for [%.3f, %.3f, %.3f]", msg->x, msg->y, msg->z);
            publish_result(false);
            return;
        }

        // Cancel previous goal before sending new one
        if (pending_goal_) {
            action_client_->async_cancel_goal(pending_goal_);
            pending_goal_ = nullptr;
        }

        // Build single-point trajectory and send directly to controller
        trajectory_msgs::msg::JointTrajectory traj;
        traj.header.stamp = this->now();
        traj.joint_names = {"joint_1", "joint_2", "joint_3", "joint_4"};

        trajectory_msgs::msg::JointTrajectoryPoint point;
        point.positions = joint_solution;
        point.time_from_start = rclcpp::Duration::from_seconds(0.15);
        traj.points.push_back(point);

        auto goal = FollowJT::Goal();
        goal.trajectory = traj;

        auto opts = rclcpp_action::Client<FollowJT>::SendGoalOptions();
        opts.goal_response_callback =
            [this](const GoalHandle::SharedPtr& handle) {
                if (handle) pending_goal_ = handle;
            };

        action_client_->async_send_goal(goal, opts);

        RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 500,
            "Sent [%.3f, %.3f, %.3f]", msg->x, msg->y, msg->z);
        publish_result(true);
    }

    void publish_result(bool success)
    {
        std_msgs::msg::Bool msg;
        msg.data = success;
        result_pub_->publish(msg);
    }

    // Subscriptions & publishers
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr waypoint_sub_;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr result_pub_;

    // Action client (direct to controller — no MoveIt planning)
    rclcpp_action::Client<FollowJT>::SharedPtr action_client_;
    GoalHandle::SharedPtr pending_goal_;

    // Robot model & IK
    moveit::core::RobotModelPtr robot_model_;
    moveit::core::RobotStatePtr robot_state_;
    std::string ee_link_;
    std::unique_ptr<GradientDescentIK> gd_ik_;

    // Joint state cache
    std::mutex js_mutex_;
    sensor_msgs::msg::JointState::SharedPtr last_joint_state_;

    bool initialized_ = false;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MoveServer>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);

    std::thread init_thread([&node]() {
        rclcpp::sleep_for(std::chrono::milliseconds(500));
        node->initialize();
    });

    executor.spin();
    init_thread.join();
    rclcpp::shutdown();
    return 0;
}
