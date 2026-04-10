#ifndef MUJOCO_SIM_PLUGINS__RESET_PLUGIN_HPP_
#define MUJOCO_SIM_PLUGINS__RESET_PLUGIN_HPP_

#include <atomic>
#include <mujoco/mujoco.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/empty.hpp>
#include "mujoco_ros2_control_plugins/mujoco_ros2_control_plugins_base.hpp"

namespace mujoco_sim_plugins
{

class ResetPlugin : public mujoco_ros2_control_plugins::MuJoCoROS2ControlPluginBase
{
public:
  bool init(rclcpp::Node::SharedPtr node, const mjModel* model, mjData* data) override;
  void update(const mjModel* model, mjData* data) override;
  void cleanup() override;

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr reset_sub_;
  rclcpp::Logger logger_{rclcpp::get_logger("reset_plugin")};
  std::atomic<bool> reset_requested_{false};
  int target_qpos_adr_{-1};  // qpos index for target_free joint (cube randomization)
};

}  // namespace mujoco_sim_plugins

#endif
