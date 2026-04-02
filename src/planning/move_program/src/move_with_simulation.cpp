#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/robot_state/robot_state.hpp>
#include <moveit/robot_model_loader/robot_model_loader.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <chrono>

#include "move_program/gradient_ik.hpp"

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto const node = std::make_shared<rclcpp::Node>(
        "move_with_simulation",
        rclcpp::NodeOptions()
            .automatically_declare_parameters_from_overrides(true)
            .append_parameter_override("use_sim_time", true)
    );

    auto const logger = rclcpp::get_logger("move_with_simulation");

    if (argc < 4) {
        RCLCPP_ERROR(logger, "Usage: ros2 run move_program move_with_simulation <x> <y> <z>");
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

    RCLCPP_INFO(logger, "Target position: [%.3f, %.3f, %.3f]", target_x, target_y, target_z);

    // Load robot model
    robot_model_loader::RobotModelLoader robot_model_loader(node);
    const moveit::core::RobotModelPtr& robot_model = robot_model_loader.getModel();
    if (!robot_model) {
        RCLCPP_ERROR(logger, "Could not load robot model!");
        rclcpp::shutdown();
        return 1;
    }

    moveit::planning_interface::MoveGroupInterface move_group(node, "arm");
    GradientDescentIK gd_ik(robot_model, "arm", move_group.getEndEffectorLink());

    // Fast movements
    move_group.setMaxVelocityScalingFactor(1.0);
    move_group.setMaxAccelerationScalingFactor(1.0);

    Eigen::Vector3d target_position(target_x, target_y, target_z);

    // Get current joint state or fall back to zeros
    rclcpp::sleep_for(std::chrono::seconds(1));
    rclcpp::spin_some(node);
    std::vector<double> joint_solution = move_group.getCurrentJointValues();
    if (joint_solution.empty()) {
        RCLCPP_WARN(logger, "Could not fetch current joint state, using zeros as initial seed");
        joint_solution.assign(robot_model->getJointModelGroup("arm")->getVariableCount(), 0.0);
    }

    // Solve IK (position-only, works with 4-DOF arm)
    if (gd_ik.solveIK(target_position, joint_solution)) {
        RCLCPP_INFO(logger, "IK solution found, planning trajectory...");

        move_group.setJointValueTarget(joint_solution);
        moveit::planning_interface::MoveGroupInterface::Plan plan;

        if (static_cast<bool>(move_group.plan(plan))) {
            RCLCPP_INFO(logger, "Plan found, executing...");
            move_group.execute(plan);
            RCLCPP_INFO(logger, "Motion complete.");
        } else {
            RCLCPP_ERROR(logger, "Trajectory planning failed.");
        }
    } else {
        RCLCPP_ERROR(logger, "IK solver failed — target may be out of reach.");
    }

    rclcpp::shutdown();
    return 0;
}
