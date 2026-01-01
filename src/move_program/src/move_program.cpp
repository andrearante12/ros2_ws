#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>

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
    moveit::planning_interface::MoveGroupInterface move_group(node, "arm");

    RCLCPP_INFO(logger, "=== MOVING TO JOINT ANGLES ===\n");

    // Get current joint positions
    std::vector<double> current_joints = move_group.getCurrentJointValues();
    RCLCPP_INFO(logger, "Current joints: [%.2f, %.2f, %.2f, %.2f] (radians)",
                current_joints[0], current_joints[1], 
                current_joints[2], current_joints[3]);

    // Define target joint angles (convert degrees to radians)
    // Example: [0, -45, -45, 0] degrees
    std::vector<double> target_joints = {
        0.0,                    // 0 degrees
        -45.0 * M_PI / 180.0,   // -45 degrees → radians
        -45.0 * M_PI / 180.0,   // -45 degrees → radians
        0.0                     // 0 degrees
    };

    RCLCPP_INFO(logger, "Target joints: [%.2f, %.2f, %.2f, %.2f] (radians)",
                target_joints[0], target_joints[1], 
                target_joints[2], target_joints[3]);
    RCLCPP_INFO(logger, "Target joints: [%.1f, %.1f, %.1f, %.1f] (degrees)\n",
                target_joints[0] * 180.0 / M_PI,
                target_joints[1] * 180.0 / M_PI,
                target_joints[2] * 180.0 / M_PI,
                target_joints[3] * 180.0 / M_PI);

    // Set the target joint angles
    move_group.setJointValueTarget(target_joints);
    move_group.setPlanningTime(5.0);

    // Create and execute the plan
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    bool success = static_cast<bool>(move_group.plan(my_plan));

    if (success) {
        RCLCPP_INFO(logger, "✓ Planning SUCCESS!");
        RCLCPP_INFO(logger, "Executing motion...");
        
        move_group.execute(my_plan);
        
        RCLCPP_INFO(logger, "✓ Motion complete!");
        
        // Verify final position
        auto final_pose = move_group.getCurrentPose();
        RCLCPP_INFO(logger, "Final Cartesian position: [%.3f, %.3f, %.3f]",
                    final_pose.pose.position.x,
                    final_pose.pose.position.y,
                    final_pose.pose.position.z);
    } else {
        RCLCPP_ERROR(logger, "✗ Planning FAILED!");
    }

    rclcpp::shutdown();
    return 0;
}