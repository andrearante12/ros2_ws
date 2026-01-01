
[move_group-8] [INFO] [1766987097.162047369] [move_group]: Calling Planner 'OMPL'
[move_group-8] [ERROR] [1766987102.166189670] [move_group.moveit.moveit.planners.ompl.planner_manager]: ./src/ompl/geometric/planners/rrt/src/RRTConnect.cpp:265 - arm/arm: Unable to sample any valid states for goal tree
[move_group-8] [WARN] [1766987102.166330222] [move_group.moveit.moveit.planners.ompl.model_based_planning_context]: Invalid goal state
[move_group-8] [ERROR] [1766987102.491438722] [move_group.moveit.moveit.planners.ompl.model_based_planning_context]: Unable to solve the planning problem
[move_group-8] [ERROR] [1766987102.491529281] [move_group]: Planner 'OMPL' failed with error code GOAL_STATE_INVALID
[move_group-8] [ERROR] [1766987102.491553522] [move_group.moveit.moveit.ros.move_group.move_action]: Generating a plan with planning pipeline failed.
[move_group-8] [INFO] [1766987102.491611602] [move_group.moveit.moveit.ros.move_group.move_action]: FAILURE
[move_group-8] [INFO] [1766987583.174792582] [move_group.moveit.moveit.ros.move_group.move_action]: MoveGroupMoveAction: Received request
[move_group-8] [INFO] [1766987583.175021480] [move_group.moveit.moveit.ros.move_group.move_action]: executing..
[move_group-8] [INFO] [1766987583.180225652] [move_group.moveit.moveit.ros.move_group.move_action]: Planning request received for MoveGroup action. Forwarding to planning pipeline.
[move_group-8] [INFO] [1766987583.180336495] [move_group]: Calling PlanningRequestAdapter 'ResolveConstraintFrames'
[move_group-8] [INFO] [1766987583.180378920] [move_group]: Calling PlanningRequestAdapter 'ValidateWorkspaceBounds'
[move_group-8] [WARN] [1766987583.180388426] [move_group.moveit.moveit.ros.validate_workspace_bounds]: It looks like the planning volume was not specified. Using default values.
[move_group-8] [INFO] [1766987583.180407738] [move_group]: Calling PlanningRequestAdapter 'CheckStartStateBounds'
[move_group-8] [INFO] [1766987583.180435456] [move_group]: Calling PlanningRequestAdapter 'CheckStartStateCollision'
[move_group-8] [INFO] [1766987583.180805374] [move_group.moveit.moveit.planners.ompl.model_based_planning_context]: Planner configuration 'arm' will use planner 'geometric::RRTConnect'. Additional configuration parameters will be set when the planner is constructed.
[move_group-8] [INFO] [1766987583.180879295] [move_group]: Calling Planner 'OMPL'
[move_group-8] [ERROR] [1766987588.182885329] [move_group.moveit.moveit.planners.ompl.planner_manager]: ./src/ompl/geometric/planners/rrt/src/RRTConnect.cpp:265 - arm/arm: Unable to sample any valid states for goal tree
[move_group-8] [WARN] [1766987588.183012254] [move_group.moveit.moveit.planners.ompl.model_based_planning_context]: Invalid goal state
[move_group-8] [ERROR] [1766987588.509001172] [move_group.moveit.moveit.planners.ompl.model_based_planning_context]: Unable to solve the planning problem
[move_group-8] [ERROR] [1766987588.509108894] [move_group]: Planner 'OMPL' failed with error code GOAL_STATE_INVALID
[move_group-8] [ERROR] [1766987588.509137250] [move_group.moveit.moveit.ros.move_group.move_action]: Generating a plan with planning pipeline failed.
[move_group-8] [INFO] [1766987588.509160071] [move_group.moveit.moveit.ros.move_group.move_action]: FAILURE

Show more
12:53 AM
This error indicates that MoveIt's OMPL planner cannot find valid goal states for your robot arm. Here are the main causes and solutions:

Common Causes
1. Goal state is in collision

The target pose causes the robot to collide with itself, obstacles, or the environment
Check if the goal position is reachable without collisions
2. Goal state is outside joint limits

The inverse kinematics solution violates joint position, velocity, or acceleration limits
Verify your URDF/SRDF joint limits are correctly configured
3. Goal state is unreachable

The target pose is outside the robot's workspace
IK solver cannot find a valid joint configuration for the desired end-effector pose
4. Invalid goal constraints

Path or goal constraints are too restrictive
Tolerance values might be too tight
Debugging Steps
python
# 1. Check if goal is valid before planning
from moveit_msgs.msg import Constraints
from moveit_commander import MoveGroupCommander

arm = MoveGroupCommander("arm")

# Test current state first
current_pose = arm.get_current_pose().pose
print(f"Current pose: {current_pose}")

# Check goal pose
goal_pose = ...  # your target pose
arm.set_pose_target(goal_pose)

# Try with relaxed constraints
arm.set_goal_position_tolerance(0.01)  # 1cm
arm.set_goal_orientation_tolerance(0.1)  # ~5.7 degrees

# 2. Verify IK solution exists
joint_values = arm.get_current_joint_values()
print(f"Current joints: {joint_values}")

# Test if IK can solve for goal
from moveit_commander import RobotCommander
robot = RobotCommander()
eef_link = arm.get_end_effector_link()

# Manual IK check would require calling IK service
Solutions
Option 1: Relax goal tolerances

python
arm.set_goal_position_tolerance(0.02)
arm.set_goal_orientation_tolerance(0.2)
arm.set_planning_time(10.0)  # Give more time
Option 2: Use joint space goals instead of pose goals

python
# Instead of pose target
# arm.set_pose_target(pose)

# Use joint target if you know valid joint values
joint_goal = [0.0, -0.5, 0.0, -1.5, 0.0, 1.0, 0.0]
arm.set_joint_value_target(joint_goal)
Option 3: Check and visualize the goal in RViz

bash
# Launch RViz with MoveIt plugin
ros2 launch your_robot_moveit_config move_group.launch.py
Use the interactive marker to manually test if the goal is reachable
Look for red/collision states in the planning scene
Option 4: Validate goal state programmatically

python
from moveit_msgs.srv import GetStateValidity

# Check if a state is valid
# (requires calling the state validity service)
Option 5: Check SRDF configuration Make sure your *.srdf file has correct:

Collision checking pairs (disable self-collision where appropriate)
Joint limits match your URDF
Planning groups are properly defined
Would you like help with any specific aspect of debugging this issue, such as checking your robot configuration or modifying your planning code?

edit this file: 

#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

int main(int argc, char * argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
    "hello_moveit",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("hello_moveit");

  // Create the MoveIt MoveGroup Interface
using moveit::planning_interface::MoveGroupInterface;
auto move_group_interface = MoveGroupInterface(node, "arm");

// Set a target Pose
auto const target_pose = []{
  geometry_msgs::msg::Pose msg;
  msg.orientation.w = 1.0;
  msg.position.x = 0.68;
  msg.position.y = -1.27;
  msg.position.z = 1.17;
  return msg;
}();
move_group_interface.setPoseTarget(target_pose);


// Create a plan to that target pose
auto const [success, plan] = [&move_group_interface]{
  moveit::planning_interface::MoveGroupInterface::Plan msg;
  auto const ok = static_cast<bool>(move_group_interface.plan(msg));
  return std::make_pair(ok, msg);
}();

// Execute the plan
if(success) {
  move_group_interface.execute(plan);
} else {
  RCLCPP_ERROR(logger, "Planing failed!");
}

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
Show more
12:54 AM
I'll help you add debugging and error handling to diagnose the invalid goal state issue:

hello_moveit.cpp - Enhanced with Debugging
Code 
I've enhanced your code with several improvements:

Key Changes:
Diagnostic Information: Prints current pose, joint values, planning frame, and end effector link to help understand the robot's state
Relaxed Goal Tolerances: Added position (1cm) and orientation (~5.7°) tolerances to make goals easier to reach
Multiple Planning Attempts: Tries planning up to 3 times before giving up
Better Error Messages: Provides specific suggestions when planning fails
Execution Verification: Checks if execution actually succeeded
Additional Troubleshooting:
If this still fails, try these alternatives:

Option 1: Use a named target (if configured)

cpp
move_group_interface.setNamedTarget("home"); // or "ready", etc.
Option 2: Use joint space goals

cpp
std::vector<double> joint_goal = {0.0, -0.5, 0.0, -1.5, 0.0, 1.0, 0.0};
move_group_interface.setJointValueTarget(joint_goal);
Option 3: Move incrementally

cpp
// Move to an intermediate pose first
auto intermediate_pose = current_pose.pose;
intermediate_pose.position.z += 0.1; // Move up 10cm first
move_group_interface.setPoseTarget(intermediate_pose);
Compile and run this to see detailed diagnostic output that will help identify why the goal is invalid!

andre@nucbox:~/ros2_ws$ ros2 run hello_moveit hello_moveit
[INFO] [1766987739.425750597] [moveit_2433313300.moveit.ros.rdf_loader]: Loaded robot model in 1.25998 seconds
[INFO] [1766987739.425877990] [moveit_2433313300.moveit.core.robot_model]: Loading robot model 'robotic_arm_model_v3'...
[WARN] [1766987739.476952545] [moveit_2433313300.moveit.ros.robot_model_loader]: No kinematics plugins defined. Fill and load kinematics.yaml!
[INFO] [1766987739.505076231] [moveit_2433313300.moveit.ros.move_group_interface]: Ready to take commands for planning group arm.
[INFO] [1766987739.505131713] [hello_moveit]: Planning frame: world
[INFO] [1766987739.505154387] [hello_moveit]: End effector link: link_4
[INFO] [1766987739.506591545] [moveit_2433313300.moveit.ros.current_state_monitor]: Listening to joint states on topic 'joint_states'
[INFO] [1766987740.506817622] [moveit_2433313300.moveit.ros.current_state_monitor]: Didn't receive robot state (joint angles) with recent timestamp within 1.000000 seconds. Requested time 1766987739.506677, but latest received state has time 0.000000.
Check clock synchronization if your are running ROS across multiple machines!
[ERROR] [1766987740.507041990] [moveit_2433313300.moveit.ros.move_group_interface]: Failed to fetch current robot state
[INFO] [1766987740.507245087] [hello_moveit]: Current pose: [0.00, 0.00, 0.00]
[INFO] [1766987741.507400894] [moveit_2433313300.moveit.ros.current_state_monitor]: Didn't receive robot state (joint angles) with recent timestamp within 1.000000 seconds. Requested time 1766987740.507306, but latest received state has time 0.000000.
Check clock synchronization if your are running ROS across multiple machines!
[ERROR] [1766987741.507586110] [moveit_2433313300.moveit.ros.move_group_interface]: Failed to fetch current robot state
[INFO] [1766987741.507932362] [hello_moveit]: Current joint values:
[INFO] [1766987741.508009351] [hello_moveit]: Goal tolerances - Position: 0.01m, Orientation: 0.1rad
[INFO] [1766987741.508050700] [hello_moveit]: Target pose: [0.68, -1.27, 1.17]
[INFO] [1766987741.508159703] [hello_moveit]: Planning attempt 1/3...
[INFO] [1766987741.508460556] [moveit_2433313300.moveit.ros.move_group_interface]: MoveGroup action client/server ready
[INFO] [1766987741.513186804] [moveit_2433313300.moveit.ros.move_group_interface]: Planning request accepted
[INFO] [1766987751.802699144] [moveit_2433313300.moveit.ros.move_group_interface]: Planning request aborted
[ERROR] [1766987751.803422207] [moveit_2433313300.moveit.ros.move_group_interface]: MoveGroupInterface::plan() failed or timeout reached
[WARN] [1766987751.803472936] [hello_moveit]: Planning attempt 1 failed
[INFO] [1766987751.803490387] [hello_moveit]: Planning attempt 2/3...
[INFO] [1766987751.803586912] [moveit_2433313300.moveit.ros.move_group_interface]: MoveGroup action client/server ready
[INFO] [1766987751.804033806] [moveit_2433313300.moveit.ros.move_group_interface]: Planning request accepted
[INFO] [1766987762.072918194] [moveit_2433313300.moveit.ros.move_group_interface]: Planning request aborted
[ERROR] [1766987762.073098152] [moveit_2433313300.moveit.ros.move_group_interface]: MoveGroupInterface::plan() failed or timeout reached
[WARN] [1766987762.073136351] [hello_moveit]: Planning attempt 2 failed
[INFO] [1766987762.073153918] [hello_moveit]: Planning attempt 3/3...
[INFO] [1766987762.073263247] [moveit_2433313300.moveit.ros.move_group_interface]: MoveGroup action client/server ready
[INFO] [1766987762.073641742] [moveit_2433313300.moveit.ros.move_group_interface]: Planning request accepted

Show more
12:56 AM
The key issue is no kinematics plugins are loaded! This is why planning fails. You also have joint state monitoring issues.

kinematics.yaml Configuration Guide
Code 
Critical Issues Found:
1. Missing Kinematics Plugin (Primary Issue)
[WARN] No kinematics plugins defined. Fill and load kinematics.yaml!
Solution: Create or update kinematics.yaml in your MoveIt config package.

2. No Joint States Being Published
Didn't receive robot state (joint angles) with recent timestamp
latest received state has time 0.000000
Solution: You need a joint state publisher running.

Fix Steps:
Step 1: Create/Update kinematics.yaml
Find your MoveIt config package:

bash
cd ~/ros2_ws/src/<your_robot>_moveit_config/config/
Create or edit kinematics.yaml with the content from the artifact above.

Step 2: Start Joint State Publisher
You need joint states. Choose ONE option:

Option A: If you have a real robot/Gazebo simulation:

bash
# Joint states should come from your robot/simulator
ros2 topic echo /joint_states  # Verify they exist
Option B: If testing without hardware (fake controllers):

bash
# Install if needed
sudo apt install ros-humble-joint-state-publisher-gui

# Run it
ros2 run joint_state_publisher_gui joint_state_publisher_gui
Option C: Add to your launch file:

python
# In your move_group.launch.py or robot.launch.py
from launch_ros.actions import Node

joint_state_publisher = Node(
    package='joint_state_publisher',
    executable='joint_state_publisher',
    name='joint_state_publisher',
    parameters=[{'source_list': ['/joint_states']}]
)

# Add to your launch description
return LaunchDescription([
    # ... other nodes ...
    joint_state_publisher,
])
Step 3: Verify kinematics.yaml is loaded
Check your move_group.launch.py:

python
# Should have something like:
moveit_config = MoveItConfigsBuilder(
    robot_name="robotic_arm_model_v3", 
    package_name="your_moveit_config"
).to_moveit_configs()

# The kinematics.yaml should be automatically loaded
# Verify it exists at: <package>/config/kinematics.yaml
Step 4: Test the fixes
bash
# Terminal 1: Launch move_group with your robot
ros2 launch your_moveit_config_package move_group.launch.py

# Terminal 2: Verify joint states
ros2 topic echo /joint_states

# Terminal 3: Run your planning node
ros2 run hello_moveit hello_moveit
Quick Diagnostic Commands:
bash
# Check if kinematics parameters are loaded
ros2 param list | grep kinematics

# Check joint states topic
ros2 topic list | grep joint
ros2 topic hz /joint_states

# Check what MoveIt sees
ros2 topic echo /move_group/display_planned_path
```

## Expected Output After Fixes:

You should see:
```
[INFO] Loaded robot model in X seconds
[INFO] Loading robot model 'robotic_arm_model_v3'...
✓ No warning about kinematics plugins!
[INFO] Using kinematics solver: kdl_kinematics_plugin/KDLKinematicsPlugin
Once these are fixed, planning should work! Let me know which issue you want to tackle first.

andre@nucbox:~/ros2_ws$ ls src/robotic_arm_v3_config/config/
bridge.yaml             moveit_controllers.yaml                  robotic_arm_model_v3.srdf
initial_positions.yaml  moveit.rviz                              robotic_arm_model_v3.urdf.xacro
joint_limits.yaml       pilz_cartesian_limits.yaml               ros2_controllers.yaml
kinematics.yaml         robotic_arm_model_v3.ros2_control.xacro  ros_gz_bridge.yaml
andre@nucbox:~/ros2_ws$ cat src/robotic_arm_v3_config/config/kinematics.yaml 
arm:
  kinematics_solver: kdl_kinematics_plugin/KDLKinematicsPlugin
  kinematics_solver_search_resolution: 0.0050000000000000001
  kinematics_solver_timeout: 0.0050000000000000001
arm:
  kinematics_solver: kdl_kinematics_plugin/KDLKinematicsPlugin
  kinematics_solver_search_resolution: 0.005
  kinematics_solver_timeout: 0.1
andre@nucbox:~/ros2_ws$

Show more
12:59 AM
I see two issues in your kinematics.yaml:

Duplicate arm: entries (the second one overwrites the first)
Timeout is too short (0.1 seconds might not be enough)
Let me fix this:

Fixed kinematics.yaml
Code 
Now update your file:

bash
# Backup the old file
cp ~/ros2_ws/src/robotic_arm_v3_config/config/kinematics.yaml ~/ros2_ws/src/robotic_arm_v3_config/config/kinematics.yaml.backup

# Replace with fixed version
cat > ~/ros2_ws/src/robotic_arm_v3_config/config/kinematics.yaml << 'EOF'
arm:
  kinematics_solver: kdl_kinematics_plugin/KDLKinematicsPlugin
  kinematics_solver_search_resolution: 0.005
  kinematics_solver_timeout: 0.5
  kinematics_solver_attempts: 3
EOF
Now let's address the joint states issue. Check if joint states are being published:

bash
# Check if topic exists
ros2 topic list | grep joint_states

# If it exists, check the data
ros2 topic echo /joint_states --once
If no joint states are published, you need to start your robot/simulation. What are you using?

Option 1: If using Gazebo/Ignition:

bash
# Start your simulation first
ros2 launch robotic_arm_v3_config <your_gazebo_launch_file>.launch.py
Option 2: If testing without simulation (demo mode):

bash
# Launch in demo mode with fake joint states
ros2 launch robotic_arm_v3_config demo.launch.py
Option 3: Manually publish fake joint states for testing:

bash
# Install if needed
sudo apt install ros-humble-joint-state-publisher

# Run it
ros2 run joint_state_publisher joint_state_publisher
After fixing kinematics.yaml and ensuring joint states are published, rebuild and test:

bash
cd ~/ros2_ws
colcon build --packages-select robotic_arm_v3_config
source install/setup.bash

# Now run your program
ros2 run hello_moveit hello_moveit
What type of setup are you using? (Gazebo simulation, real robot, or just testing motion planning?)

testing without simulation: [INFO] [spawner-7]: process has finished cleanly [pid 120072]
[move_group-8] [INFO] [1766988269.330913088] [move_group.moveit.moveit.ros.move_group.move_action]: MoveGroupMoveAction: Received request
[move_group-8] [INFO] [1766988269.331433098] [move_group.moveit.moveit.ros.move_group.move_action]: executing..
[move_group-8] [INFO] [1766988269.337525241] [move_group.moveit.moveit.ros.move_group.move_action]: Planning request received for MoveGroup action. Forwarding to planning pipeline.
[move_group-8] [INFO] [1766988269.337960629] [move_group]: Calling PlanningRequestAdapter 'ResolveConstraintFrames'
[move_group-8] [INFO] [1766988269.338076367] [move_group]: Calling PlanningRequestAdapter 'ValidateWorkspaceBounds'
[move_group-8] [WARN] [1766988269.338090852] [move_group.moveit.moveit.ros.validate_workspace_bounds]: It looks like the planning volume was not specified. Using default values.
[move_group-8] [INFO] [1766988269.338119241] [move_group]: Calling PlanningRequestAdapter 'CheckStartStateBounds'
[move_group-8] [INFO] [1766988269.338161819] [move_group]: Calling PlanningRequestAdapter 'CheckStartStateCollision'
[move_group-8] [INFO] [1766988269.339272256] [move_group.moveit.moveit.planners.ompl.model_based_planning_context]: Planner configuration 'arm' will use planner 'geometric::RRTConnect'. Additional configuration parameters will be set when the planner is constructed.
[move_group-8] [INFO] [1766988269.339535672] [move_group]: Calling Planner 'OMPL'
[move_group-8] [ERROR] [1766988274.343368732] [move_group.moveit.moveit.planners.ompl.planner_manager]: ./src/ompl/geometric/planners/rrt/src/RRTConnect.cpp:265 - arm/arm: Unable to sample any valid states for goal tree
[move_group-8] [WARN] [1766988274.343533845] [move_group.moveit.moveit.planners.ompl.model_based_planning_context]: Invalid goal state
[move_group-8] [ERROR] [1766988275.370365148] [move_group.moveit.moveit.planners.ompl.model_based_planning_context]: Unable to solve the planning problem
[move_group-8] [ERROR] [1766988275.370458875] [move_group]: Planner 'OMPL' failed with error code GOAL_STATE_INVALID
[move_group-8] [ERROR] [1766988275.370483608] [move_group.moveit.moveit.ros.move_group.move_action]: Generating a plan with planning pipeline failed.
[move_group-8] [INFO] [1766988275.370551489] [move_group.moveit.moveit.ros.move_group.move_action]: FAILURE

Show more
1:05 AM
Good! Kinematics is now loaded. The issue is that your goal pose is unreachable or causes collisions. The target [0.68, -1.27, 1.17] is likely outside your robot's workspace.

Let's create a more comprehensive test program that:

Tests if the goal is reachable
Tries different goals
Shows workspace limits
hello_moveit.cpp - Enhanced with Debugging
Code 
Now rebuild and test:

bash
cd ~/ros2_ws
colcon build --packages-select hello_moveit
source install/setup.bash

# Make sure move_group is running
ros2 launch robotic_arm_v3_config move_group.launch.py

# In another terminal, run the diagnostic
ros2 run hello_moveit hello_moveit
This will tell you:

✓ or ✗ if IK solutions exist for different





Claude is AI and can make mistakes. Please double-check responses.
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_state/robot_state.h>

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
    "hello_moveit",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );

  auto const logger = rclcpp::get_logger("hello_moveit");

  using moveit::planning_interface::MoveGroupInterface;
  auto move_group_interface = MoveGroupInterface(node, "arm");

  // Print robot information
  RCLCPP_INFO(logger, "=== Robot Information ===");
  RCLCPP_INFO(logger, "Planning frame: %s", move_group_interface.getPlanningFrame().c_str());
  RCLCPP_INFO(logger, "End effector link: %s", move_group_interface.getEndEffectorLink().c_str());
  
  auto current_pose = move_group_interface.getCurrentPose();
  RCLCPP_INFO(logger, "\n=== Current State ===");
  RCLCPP_INFO(logger, "Current pose: [%.3f, %.3f, %.3f]", 
              current_pose.pose.position.x,
              current_pose.pose.position.y,
              current_pose.pose.position.z);
  
  auto current_joints = move_group_interface.getCurrentJointValues();
  RCLCPP_INFO(logger, "Current joint values:");
  for (size_t i = 0; i < current_joints.size(); ++i) {
    RCLCPP_INFO(logger, "  Joint %zu: %.3f rad (%.1f deg)", i, current_joints[i], current_joints[i] * 180.0 / M_PI);
  }

  // Load robot model to check IK
  robot_model_loader::RobotModelLoader robot_model_loader(node);
  const moveit::core::RobotModelPtr& kinematic_model = robot_model_loader.getModel();
  moveit::core::RobotStatePtr kinematic_state(new moveit::core::RobotState(kinematic_model));
  kinematic_state->setToDefaultValues();
  
  const moveit::core::JointModelGroup* joint_model_group = 
    kinematic_model->getJointModelGroup("arm");

  // Test original goal
  RCLCPP_INFO(logger, "\n=== Testing Original Goal ===");
  geometry_msgs::msg::Pose original_goal;
  original_goal.orientation.w = 1.0;
  original_goal.position.x = 0.68;
  original_goal.position.y = -1.27;
  original_goal.position.z = 1.17;
  
  RCLCPP_INFO(logger, "Original goal: [%.3f, %.3f, %.3f]", 
              original_goal.position.x,
              original_goal.position.y,
              original_goal.position.z);
  
  bool found_ik = kinematic_state->setFromIK(joint_model_group, original_goal, 1.0);
  if (found_ik) {
    RCLCPP_INFO(logger, "✓ IK solution EXISTS for original goal");
  } else {
    RCLCPP_ERROR(logger, "✗ NO IK solution for original goal - it's unreachable!");
  }

  // Test a closer, safer goal
  RCLCPP_INFO(logger, "\n=== Testing Safe Goal (closer to robot) ===");
  geometry_msgs::msg::Pose safe_goal = current_pose.pose;
  safe_goal.position.x = 0.3;
  safe_goal.position.y = 0.0;
  safe_goal.position.z = 0.5;
  safe_goal.orientation.w = 1.0;
  safe_goal.orientation.x = 0.0;
  safe_goal.orientation.y = 0.0;
  safe_goal.orientation.z = 0.0;
  
  RCLCPP_INFO(logger, "Safe goal: [%.3f, %.3f, %.3f]", 
              safe_goal.position.x,
              safe_goal.position.y,
              safe_goal.position.z);
  
  found_ik = kinematic_state->setFromIK(joint_model_group, safe_goal, 1.0);
  if (found_ik) {
    RCLCPP_INFO(logger, "✓ IK solution EXISTS for safe goal");
    
    // Try planning to safe goal
    RCLCPP_INFO(logger, "\n=== Attempting to plan to safe goal ===");
    move_group_interface.setPoseTarget(safe_goal);
    move_group_interface.setGoalPositionTolerance(0.01);
    move_group_interface.setGoalOrientationTolerance(0.1);
    move_group_interface.setPlanningTime(10.0);
    
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = static_cast<bool>(move_group_interface.plan(plan));
    
    if (success) {
      RCLCPP_INFO(logger, "✓ Planning SUCCEEDED to safe goal!");
      RCLCPP_INFO(logger, "Execute the plan? (This would move the robot)");
      
      // Uncomment to execute:
      // auto execute_result = move_group_interface.execute(plan);
      // if (execute_result == moveit::core::MoveItErrorCode::SUCCESS) {
      //   RCLCPP_INFO(logger, "✓ Execution succeeded!");
      // }
    } else {
      RCLCPP_ERROR(logger, "✗ Planning failed even for safe goal");
    }
  } else {
    RCLCPP_ERROR(logger, "✗ NO IK solution even for safe goal");
  }

  // Test moving in joint space (most reliable)
  RCLCPP_INFO(logger, "\n=== Testing Joint Space Motion ===");
  std::vector<double> joint_goal = current_joints;
  // Small movement: add 0.1 rad (~5.7 deg) to first joint
  joint_goal[0] += 0.1;
  
  RCLCPP_INFO(logger, "Joint goal: moving joint 0 by 0.1 rad");
  move_group_interface.setJointValueTarget(joint_goal);
  
  moveit::planning_interface::MoveGroupInterface::Plan joint_plan;
  bool joint_success = static_cast<bool>(move_group_interface.plan(joint_plan));
  
  if (joint_success) {
    RCLCPP_INFO(logger, "✓ Joint space planning SUCCEEDED!");
    RCLCPP_INFO(logger, "This is the most reliable planning method.");
  } else {
    RCLCPP_ERROR(logger, "✗ Even joint space planning failed");
  }

  // Summary
  RCLCPP_INFO(logger, "\n=== SUMMARY ===");
  RCLCPP_INFO(logger, "Your original goal [0.68, -1.27, 1.17] is likely:");
  RCLCPP_INFO(logger, "  1. Outside the robot's reach");
  RCLCPP_INFO(logger, "  2. In collision with the robot or environment");
  RCLCPP_INFO(logger, "\nRecommendations:");
  RCLCPP_INFO(logger, "  • Use RViz interactive markers to find valid goals");
  RCLCPP_INFO(logger, "  • Start with small movements from current position");
  RCLCPP_INFO(logger, "  • Use joint space goals for reliability");
  RCLCPP_INFO(logger, "  • Check your URDF link lengths and limits");

  rclcpp::shutdown();
  return 0;
}

