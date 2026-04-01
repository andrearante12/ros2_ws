#pragma once

#include <vector>
#include <cmath>
#include <limits>
#include <rclcpp/rclcpp.hpp>
#include <moveit/robot_model/robot_model.hpp>
#include <moveit/robot_state/robot_state.hpp>

/**
 * Gradient descent inverse kinematics solver.
 *
 * Numerically minimises the squared Cartesian distance between the
 * end-effector and a target position by iteratively updating joint angles
 * in the direction of steepest descent.
 */
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
