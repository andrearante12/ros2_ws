#pragma message("#include \"local_planner_parameters.hpp\" is deprecated. Use #include <moveit_hybrid_planning/local_planner_parameters.hpp> instead.")
// auto-generated DO NOT EDIT

#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <limits>
#include <mutex>
#include <rclcpp/node.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <rclcpp/logger.hpp>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <parameter_traits/parameter_traits.hpp>

#include <rsl/static_string.hpp>
#include <rsl/static_vector.hpp>
#include <rsl/parameter_validators.hpp>



namespace local_planner_parameters {

// Use validators from RSL
using rsl::unique;
using rsl::subset_of;
using rsl::fixed_size;
using rsl::size_gt;
using rsl::size_lt;
using rsl::not_empty;
using rsl::element_bounds;
using rsl::lower_element_bounds;
using rsl::upper_element_bounds;
using rsl::bounds;
using rsl::lt;
using rsl::gt;
using rsl::lt_eq;
using rsl::gt_eq;
using rsl::one_of;
using rsl::to_parameter_result_msg;

// temporarily needed for backwards compatibility for custom validators
using namespace parameter_traits;

template <typename T>
[[nodiscard]] auto to_parameter_value(T value) {
    return rclcpp::ParameterValue(value);
}

template <size_t capacity>
[[nodiscard]] auto to_parameter_value(rsl::StaticString<capacity> const& value) {
    return rclcpp::ParameterValue(rsl::to_string(value));
}

template <typename T, size_t capacity>
[[nodiscard]] auto to_parameter_value(rsl::StaticVector<T, capacity> const& value) {
    return rclcpp::ParameterValue(rsl::to_vector(value));
}
    struct Params {
        std::string group_name = "UNKNOWN";
        std::string trajectory_operator_plugin_name = "UNKNOWN";
        std::string local_constraint_solver_plugin_name = "UNKNOWN";
        double local_planning_frequency = 1.0;
        std::string global_solution_topic = "UNKNOWN";
        std::string local_solution_topic = "UNKNOWN";
        std::string local_solution_topic_type = "UNKNOWN";
        bool publish_joint_positions = false;
        bool publish_joint_velocities = false;
        std::string monitored_planning_scene_topic = "/planning_scene";
        std::string collision_object_topic = "/collision_object";
        std::string joint_states_topic = "/joint_states";
        // for detecting if the parameter struct has been updated
        rclcpp::Time __stamp;
    };
    struct StackParams {
        double local_planning_frequency = 1.0;
        bool publish_joint_positions = false;
        bool publish_joint_velocities = false;
    };

  class ParamListener{
  public:
    // throws rclcpp::exceptions::InvalidParameterValueException on initialization if invalid parameter are loaded
    ParamListener(rclcpp::Node::SharedPtr node, std::string const& prefix = "")
    : ParamListener(node->get_node_parameters_interface(), node->get_logger(), prefix) {}

    ParamListener(rclcpp_lifecycle::LifecycleNode::SharedPtr node, std::string const& prefix = "")
    : ParamListener(node->get_node_parameters_interface(), node->get_logger(), prefix) {}

    ParamListener(const std::shared_ptr<rclcpp::node_interfaces::NodeParametersInterface>& parameters_interface,
                  std::string const& prefix = "")
    : ParamListener(parameters_interface, rclcpp::get_logger("local_planner_parameters"), prefix) {
      RCLCPP_DEBUG(logger_, "ParameterListener: Not using node logger, recommend using other constructors to use a node logger");
    }

    ParamListener(const std::shared_ptr<rclcpp::node_interfaces::NodeParametersInterface>& parameters_interface,
                  rclcpp::Logger logger, std::string const& prefix = "")
    : prefix_{prefix},
      logger_{std::move(logger)} {
      if (!prefix_.empty() && prefix_.back() != '.') {
        prefix_ += ".";
      }

      parameters_interface_ = parameters_interface;
      declare_params();
      auto update_param_cb = [this](const std::vector<rclcpp::Parameter> &parameters){return this->update(parameters);};
      handle_ = parameters_interface_->add_on_set_parameters_callback(update_param_cb);
      clock_ = rclcpp::Clock();
    }

    Params get_params() const{
      std::lock_guard<std::mutex> lock(mutex_);
      return params_;
    }

    /**
     * @brief Tries to update the parsed Params object
     * @param params_in The Params object to update
     * @return true if the Params object was updated, false if it was already up to date or the mutex could not be locked
     * @note This function tries to lock the mutex without blocking, so it can be used in a RT loop
     */
    bool try_update_params(Params & params_in) const {
      std::unique_lock<std::mutex> lock(mutex_, std::try_to_lock);
      if (lock.owns_lock()) {
        if (const bool is_old = params_in.__stamp != params_.__stamp; is_old) {
          params_in = params_;
          return true;
        }
      }
      return false;
    }

    /**
     * @brief Tries to get the current Params object
     * @param params_in The Params object to fill with the current parameters
     * @return true if mutex can be locked, false if mutex could not be locked
     * @note The parameters are only filled, when the mutex can be locked and the params timestamp is different
     * @note This function tries to lock the mutex without blocking, so it can be used in a RT loop
     */
    bool try_get_params(Params & params_in) const {
      if (mutex_.try_lock()) {
        if (const bool is_old = params_in.__stamp != params_.__stamp; is_old) {
          params_in = params_;
        }
        mutex_.unlock();
        return true;
      }
      return false;
    }

    bool is_old(Params const& other) const {
      std::lock_guard<std::mutex> lock(mutex_);
      return params_.__stamp != other.__stamp;
    }

    StackParams get_stack_params() {
      Params params = get_params();
      StackParams output;
      output.local_planning_frequency = params.local_planning_frequency;
      output.publish_joint_positions = params.publish_joint_positions;
      output.publish_joint_velocities = params.publish_joint_velocities;

      return output;
    }

    void refresh_dynamic_parameters() {
      auto updated_params = get_params();
      // TODO remove any destroyed dynamic parameters

      // declare any new dynamic parameters
      rclcpp::Parameter param;

    }

    rcl_interfaces::msg::SetParametersResult update(const std::vector<rclcpp::Parameter> &parameters) {
      auto updated_params = get_params();

      for (const auto &param: parameters) {
        if (param.get_name() == (prefix_ + "group_name")) {
            updated_params.group_name = param.as_string();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "trajectory_operator_plugin_name")) {
            updated_params.trajectory_operator_plugin_name = param.as_string();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "local_constraint_solver_plugin_name")) {
            updated_params.local_constraint_solver_plugin_name = param.as_string();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "local_planning_frequency")) {
            updated_params.local_planning_frequency = param.as_double();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "global_solution_topic")) {
            updated_params.global_solution_topic = param.as_string();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "local_solution_topic")) {
            updated_params.local_solution_topic = param.as_string();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "local_solution_topic_type")) {
            updated_params.local_solution_topic_type = param.as_string();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "publish_joint_positions")) {
            updated_params.publish_joint_positions = param.as_bool();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "publish_joint_velocities")) {
            updated_params.publish_joint_velocities = param.as_bool();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "monitored_planning_scene_topic")) {
            updated_params.monitored_planning_scene_topic = param.as_string();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "collision_object_topic")) {
            updated_params.collision_object_topic = param.as_string();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
        if (param.get_name() == (prefix_ + "joint_states_topic")) {
            updated_params.joint_states_topic = param.as_string();
            RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
        }
      }

      updated_params.__stamp = clock_.now();
      update_internal_params(updated_params);
      if (user_callback_) {
         user_callback_(updated_params);
      }
      return rsl::to_parameter_result_msg({});
    }

    void declare_params(){
      auto updated_params = get_params();
      // declare all parameters and give default values to non-required ones
      if (!parameters_interface_->has_parameter(prefix_ + "group_name")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Planning group to be used";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.group_name);
          parameters_interface_->declare_parameter(prefix_ + "group_name", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "trajectory_operator_plugin_name")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Trajectory operator plugin";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.trajectory_operator_plugin_name);
          parameters_interface_->declare_parameter(prefix_ + "trajectory_operator_plugin_name", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "local_constraint_solver_plugin_name")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Local constraint solver plugin";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.local_constraint_solver_plugin_name);
          parameters_interface_->declare_parameter(prefix_ + "local_constraint_solver_plugin_name", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "local_planning_frequency")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Spinning frequency of the local planner [Hz].";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.local_planning_frequency);
          parameters_interface_->declare_parameter(prefix_ + "local_planning_frequency", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "global_solution_topic")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Name of the topic where the global solution is published";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.global_solution_topic);
          parameters_interface_->declare_parameter(prefix_ + "global_solution_topic", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "local_solution_topic")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Name of the topic where the local solution is published";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.local_solution_topic);
          parameters_interface_->declare_parameter(prefix_ + "local_solution_topic", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "local_solution_topic_type")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Topic type of local solution";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.local_solution_topic_type);
          parameters_interface_->declare_parameter(prefix_ + "local_solution_topic_type", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "publish_joint_positions")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Whether or not the local solver publishes position commands";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.publish_joint_positions);
          parameters_interface_->declare_parameter(prefix_ + "publish_joint_positions", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "publish_joint_velocities")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Whether or not the local solver publishes velocity commands";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.publish_joint_velocities);
          parameters_interface_->declare_parameter(prefix_ + "publish_joint_velocities", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "monitored_planning_scene_topic")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Name of the topic where the reference planning scene is published";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.monitored_planning_scene_topic);
          parameters_interface_->declare_parameter(prefix_ + "monitored_planning_scene_topic", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "collision_object_topic")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Name of the topic where the collision objects are published";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.collision_object_topic);
          parameters_interface_->declare_parameter(prefix_ + "collision_object_topic", parameter, descriptor);
      }
      if (!parameters_interface_->has_parameter(prefix_ + "joint_states_topic")) {
          rcl_interfaces::msg::ParameterDescriptor descriptor;
          descriptor.description = "Name of the topic where the joint states are published";
          descriptor.read_only = false;
          auto parameter = to_parameter_value(updated_params.joint_states_topic);
          parameters_interface_->declare_parameter(prefix_ + "joint_states_topic", parameter, descriptor);
      }
      // get parameters and fill struct fields
      rclcpp::Parameter param;
      param = parameters_interface_->get_parameter(prefix_ + "group_name");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.group_name = param.as_string();
      param = parameters_interface_->get_parameter(prefix_ + "trajectory_operator_plugin_name");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.trajectory_operator_plugin_name = param.as_string();
      param = parameters_interface_->get_parameter(prefix_ + "local_constraint_solver_plugin_name");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.local_constraint_solver_plugin_name = param.as_string();
      param = parameters_interface_->get_parameter(prefix_ + "local_planning_frequency");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.local_planning_frequency = param.as_double();
      param = parameters_interface_->get_parameter(prefix_ + "global_solution_topic");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.global_solution_topic = param.as_string();
      param = parameters_interface_->get_parameter(prefix_ + "local_solution_topic");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.local_solution_topic = param.as_string();
      param = parameters_interface_->get_parameter(prefix_ + "local_solution_topic_type");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.local_solution_topic_type = param.as_string();
      param = parameters_interface_->get_parameter(prefix_ + "publish_joint_positions");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.publish_joint_positions = param.as_bool();
      param = parameters_interface_->get_parameter(prefix_ + "publish_joint_velocities");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.publish_joint_velocities = param.as_bool();
      param = parameters_interface_->get_parameter(prefix_ + "monitored_planning_scene_topic");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.monitored_planning_scene_topic = param.as_string();
      param = parameters_interface_->get_parameter(prefix_ + "collision_object_topic");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.collision_object_topic = param.as_string();
      param = parameters_interface_->get_parameter(prefix_ + "joint_states_topic");
      RCLCPP_DEBUG_STREAM(logger_, param.get_name() << ": " << param.get_type_name() << " = " << param.value_to_string());
      updated_params.joint_states_topic = param.as_string();


      updated_params.__stamp = clock_.now();
      update_internal_params(updated_params);
    }

    using userParameterUpdateCB = std::function<void(const Params&)>;
    void setUserCallback(const userParameterUpdateCB& callback){
      user_callback_ = callback;
    }

    void clearUserCallback(){
      user_callback_ = {};
    }

    private:
      void update_internal_params(Params updated_params) {
        std::lock_guard<std::mutex> lock(mutex_);
        params_ = std::move(updated_params);
      }

      std::string prefix_;
      Params params_;
      rclcpp::Clock clock_;
      std::shared_ptr<rclcpp::node_interfaces::OnSetParametersCallbackHandle> handle_;
      std::shared_ptr<rclcpp::node_interfaces::NodeParametersInterface> parameters_interface_;
      userParameterUpdateCB user_callback_;

      rclcpp::Logger logger_;
      std::mutex mutable mutex_;
  };

} // namespace local_planner_parameters
