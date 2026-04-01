#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/robot_state/robot_state.hpp>
#include <moveit/robot_model_loader/robot_model_loader.hpp>
#include <std_msgs/msg/string.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <chrono>

#include "move_program/gradient_ik.hpp"

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto const node = std::make_shared<rclcpp::Node>(
        "move_program",
        rclcpp::NodeOptions()
            .automatically_declare_parameters_from_overrides(true)
            .append_parameter_override("use_sim_time", true)
    );

    auto const logger = rclcpp::get_logger("move_program");

    if (argc < 4) {
        RCLCPP_ERROR(logger, "Usage: ros2 run move_program move_program <x> <y> <z>");
        RCLCPP_ERROR(logger, "Example: ros2 run move_program move_program 0.6 -1.2 1.145");
        rclcpp::shutdown();
        return 1;
    }

    double target_x, target_y, target_z;
    try {
        target_x = std::stod(argv[1]);
        target_y = std::stod(argv[2]);
        target_z = std::stod(argv[3]);
    } catch (const std::exception& e) {
        RCLCPP_ERROR(logger, "Invalid coordinates: %s", e.what());
        rclcpp::shutdown();
        return 1;
    }

    RCLCPP_INFO(logger, "Target Position: X=%.3f, Y=%.3f, Z=%.3f", target_x, target_y, target_z);

    // Publisher for servo commands — pose_printer holds the serial port
    auto cmd_pub = node->create_publisher<std_msgs::msg::String>("/arm/servo_commands", 10);

    // Load Robot Model
    robot_model_loader::RobotModelLoader robot_model_loader(node);
    const moveit::core::RobotModelPtr& robot_model = robot_model_loader.getModel();
    if (!robot_model) {
        RCLCPP_ERROR(logger, "Could not load robot model!");
        rclcpp::shutdown();
        return 1;
    }

    moveit::planning_interface::MoveGroupInterface move_group(node, "arm");
    GradientDescentIK gd_ik(robot_model, "arm", move_group.getEndEffectorLink());

    Eigen::Vector3d target_position(target_x, target_y, target_z);
    std::vector<double> joint_solution = move_group.getCurrentJointValues();

    if (gd_ik.solveIK(target_position, joint_solution)) {
        RCLCPP_INFO(logger, "IK Solution found!");

        // Build a single message with all servo commands
        std::string commands;
        for (size_t i = 0; i < joint_solution.size() - 1; i++) {
            int angle = static_cast<int>(std::round(joint_solution[i] * 180.0 / M_PI));
            std::string cmd = "servo" + std::to_string(i) + "=" + std::to_string(angle);
            std::cout << cmd << std::endl;
            commands += cmd + "\n";
        }

        // Wait briefly for pose_printer subscriber to connect, then publish
        rclcpp::sleep_for(std::chrono::milliseconds(500));
        std_msgs::msg::String msg;
        msg.data = commands;
        cmd_pub->publish(msg);
        rclcpp::spin_some(node);

        RCLCPP_INFO(logger, "Commands published to /arm/servo_commands.");
    } else {
        RCLCPP_ERROR(logger, "IK solver failed to find a valid solution.");
    }

    rclcpp::shutdown();
    return 0;
}
