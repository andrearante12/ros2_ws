#include "mujoco_sim_plugins/reset_plugin.hpp"
#include <pluginlib/class_list_macros.hpp>

namespace mujoco_sim_plugins
{

bool ResetPlugin::init(rclcpp::Node::SharedPtr node, const mjModel* /*model*/, mjData* /*data*/)
{
  node_ = node;
  logger_ = node_->get_logger().get_child("reset_plugin");

  reset_sub_ = node_->create_subscription<std_msgs::msg::Empty>(
    "/sim/reset", 10,
    [this](const std_msgs::msg::Empty::SharedPtr) {
      RCLCPP_INFO(logger_, "Reset requested");
      reset_requested_ = true;
    });

  RCLCPP_INFO(logger_, "Reset plugin ready — publish to /sim/reset to reset simulation");
  return true;
}

void ResetPlugin::update(const mjModel* model, mjData* data)
{
  if (reset_requested_.exchange(false)) {
    mj_resetData(model, data);
    mj_forward(model, data);
    RCLCPP_INFO(logger_, "Simulation reset complete");
  }
}

void ResetPlugin::cleanup()
{
  RCLCPP_INFO(logger_, "Reset plugin cleanup");
  reset_sub_.reset();
  node_.reset();
}

}  // namespace mujoco_sim_plugins

PLUGINLIB_EXPORT_CLASS(
  mujoco_sim_plugins::ResetPlugin,
  mujoco_ros2_control_plugins::MuJoCoROS2ControlPluginBase
)
