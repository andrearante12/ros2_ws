#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>

int main(int argc, char * argv[]) 
{
    rclcpp::init(argc, argv);
    // In your C++ main function
    auto const node = std::make_shared<rclcpp::Node>(
        "move_program",
        rclcpp::NodeOptions()
            .automatically_declare_parameters_from_overrides(true)
            .append_parameter_override("use_sim_time", true) // FORCE SIM TIME
    );

    auto const logger = rclcpp::get_logger("move_program");
    moveit::planning_interface::MoveGroupInterface move_group_interface(node, "arm");

    // 1. Get Current Pose (To see what valid coordinates look like)
    auto current_pose = move_group_interface.getCurrentPose();
    RCLCPP_INFO(logger, "Current Position: x=%f, y=%f, z=%f", 
                current_pose.pose.position.x, 
                current_pose.pose.position.y, 
                current_pose.pose.position.z);

    // 2. Set Reference Frame (Match the frame from the current pose)
    std::string planning_frame = move_group_interface.getPlanningFrame();
    move_group_interface.setPoseReferenceFrame(planning_frame);
    RCLCPP_INFO(logger, "Planning in frame: %s", planning_frame.c_str());

    // 3. Set Position Target (Ignoring Orientation)
    // Try a small increment from your current position if 0.3, 0.0, 0.5 still fails
    double target_x = 0.3;
    double target_y = 0.0;
    double target_z = 0.5;
    
    move_group_interface.setPositionTarget(target_x, target_y, target_z);

    // 4. Increase planning tolerance and time
    move_group_interface.setGoalPositionTolerance(0.01); // 1cm
    move_group_interface.setPlanningTime(10.0);

    // 5. Plan and Execute
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    auto const success = static_cast<bool>(move_group_interface.plan(my_plan));

    if (success) 
    {
        RCLCPP_INFO(logger, "Success! Moving to position...");
        move_group_interface.execute(my_plan);
    }
    else 
    {
        RCLCPP_ERROR(logger, "Planning failed. Target [%f, %f, %f] unreachable.", target_x, target_y, target_z);
    }

    rclcpp::shutdown();
    return 0;
}