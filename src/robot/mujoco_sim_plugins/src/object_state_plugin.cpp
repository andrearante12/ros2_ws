#include "mujoco_sim_plugins/object_state_plugin.hpp"
#include <pluginlib/class_list_macros.hpp>

namespace mujoco_sim_plugins
{

bool ObjectStatePlugin::init(rclcpp::Node::SharedPtr node, const mjModel* model, mjData* /*data*/)
{
  node_ = node;
  logger_ = node_->get_logger().get_child("object_state_plugin");

  // Find the target_free freejoint in the model
  int joint_id = mj_name2id(model, mjOBJ_JOINT, "target_free");
  if (joint_id < 0) {
    RCLCPP_ERROR(logger_, "Joint 'target_free' not found in MuJoCo model");
    return false;
  }
  qpos_adr_ = model->jnt_qposadr[joint_id];

  pub_ = node_->create_publisher<geometry_msgs::msg::Point>("/object/position", 10);

  RCLCPP_INFO(logger_, "Object state plugin ready — publishing to /object/position (qpos_adr=%d)", qpos_adr_);
  return true;
}

void ObjectStatePlugin::update(const mjModel* /*model*/, mjData* data)
{
  // MuJoCo runs at 1kHz, publish at ~10 Hz (every 100 steps)
  if (++update_count_ < 100) return;
  update_count_ = 0;

  geometry_msgs::msg::Point msg;
  msg.x = data->qpos[qpos_adr_ + 0];
  msg.y = data->qpos[qpos_adr_ + 1];
  msg.z = data->qpos[qpos_adr_ + 2];
  pub_->publish(msg);
}

void ObjectStatePlugin::cleanup()
{
  RCLCPP_INFO(logger_, "Object state plugin cleanup");
  pub_.reset();
  node_.reset();
}

}  // namespace mujoco_sim_plugins

PLUGINLIB_EXPORT_CLASS(
  mujoco_sim_plugins::ObjectStatePlugin,
  mujoco_ros2_control_plugins::MuJoCoROS2ControlPluginBase
)
