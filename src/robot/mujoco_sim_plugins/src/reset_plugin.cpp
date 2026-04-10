#include "mujoco_sim_plugins/reset_plugin.hpp"
#include <pluginlib/class_list_macros.hpp>
#include <random>

namespace mujoco_sim_plugins
{

bool ResetPlugin::init(rclcpp::Node::SharedPtr node, const mjModel* model, mjData* /*data*/)
{
  node_ = node;
  logger_ = node_->get_logger().get_child("reset_plugin");

  // Find the target_free freejoint for cube randomization
  int joint_id = mj_name2id(model, mjOBJ_JOINT, "target_free");
  if (joint_id >= 0) {
    target_qpos_adr_ = model->jnt_qposadr[joint_id];
    RCLCPP_INFO(logger_, "Found target_free joint (qpos_adr=%d) — cube will be randomized on reset", target_qpos_adr_);
  } else {
    RCLCPP_WARN(logger_, "Joint 'target_free' not found — cube randomization disabled");
  }

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

    // Randomize cube position on the table surface
    if (target_qpos_adr_ >= 0) {
      static std::mt19937 rng(std::random_device{}());
      static std::uniform_real_distribution<double> dist_x(-0.05, 0.05);
      static std::uniform_real_distribution<double> dist_y(0.10, 0.22);

      data->qpos[target_qpos_adr_ + 0] = dist_x(rng);
      data->qpos[target_qpos_adr_ + 1] = dist_y(rng);
      // z stays at default (on table surface), quaternion stays at identity
      RCLCPP_INFO(logger_, "Cube randomized to (%.3f, %.3f)",
                  data->qpos[target_qpos_adr_ + 0], data->qpos[target_qpos_adr_ + 1]);
    }

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
