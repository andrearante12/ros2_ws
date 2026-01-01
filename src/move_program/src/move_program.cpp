#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/robot_state/robot_state.hpp>
#include <moveit/robot_model_loader/robot_model_loader.hpp>
#include <cmath>
#include <iostream>
#include <string>

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

    // --- POSITIONAL ARGUMENT PARSING ---
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
        RCLCPP_ERROR(logger, "Invalid coordinates provided. Please use numbers. Error: %s", e.what());
        rclcpp::shutdown();
        return 1;
    }

    RCLCPP_INFO(logger, "Target Position Received: X: %.3f, Y: %.3f, Z: %.3f", target_x, target_y, target_z);

    // Load Robot Model
    robot_model_loader::RobotModelLoader robot_model_loader(node);
    const moveit::core::RobotModelPtr& robot_model = robot_model_loader.getModel();
    if (!robot_model) {
        RCLCPP_ERROR(logger, "Could not load robot model!");
        return 1;
    }

    moveit::planning_interface::MoveGroupInterface move_group(node, "arm");
    GradientDescentIK gd_ik(robot_model, "arm", move_group.getEndEffectorLink());
    
    Eigen::Vector3d target_position(target_x, target_y, target_z);
    std::vector<double> joint_solution = move_group.getCurrentJointValues();

    // Solve IK
    if (gd_ik.solveIK(target_position, joint_solution)) {
        RCLCPP_INFO(logger, "IK Solution found! Planning motion...");
        
        move_group.setJointValueTarget(joint_solution);
        moveit::planning_interface::MoveGroupInterface::Plan my_plan;
        
        if (static_cast<bool>(move_group.plan(my_plan))) {
            move_group.execute(my_plan);
            RCLCPP_INFO(logger, "Motion successful.");
        } else {
            RCLCPP_ERROR(logger, "Planning failed.");
        }
    } else {
        RCLCPP_ERROR(logger, "IK solver failed to find a valid solution.");
    }

    rclcpp::shutdown();
    return 0;
}