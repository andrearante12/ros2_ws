#ifndef MUJOCO_SIM_PLUGINS__OBJECT_STATE_PLUGIN_HPP_
#define MUJOCO_SIM_PLUGINS__OBJECT_STATE_PLUGIN_HPP_

#include <mujoco/mujoco.h>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point.hpp>
#include "mujoco_ros2_control_plugins/mujoco_ros2_control_plugins_base.hpp"

namespace mujoco_sim_plugins
{

class ObjectStatePlugin : public mujoco_ros2_control_plugins::MuJoCoROS2ControlPluginBase
{
public:
  bool init(rclcpp::Node::SharedPtr node, const mjModel* model, mjData* data) override;
  void update(const mjModel* model, mjData* data) override;
  void cleanup() override;

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr pub_;
  rclcpp::Logger logger_{rclcpp::get_logger("object_state_plugin")};
  int qpos_adr_{-1};       // qpos index for target_free joint
  int update_count_{0};     // throttle publishing to ~10 Hz
};

}  // namespace mujoco_sim_plugins

#endif
