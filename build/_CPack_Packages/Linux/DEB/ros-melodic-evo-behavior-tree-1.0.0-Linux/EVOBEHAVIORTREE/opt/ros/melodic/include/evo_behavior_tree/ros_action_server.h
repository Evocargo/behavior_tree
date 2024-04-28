#pragma once

#include <actionlib/server/simple_action_server.h>
#include <ros/ros.h>

#include "ros_action_alias.h"

namespace evo::behavior {

template <typename RosActionType> class RosActionServer {
public:
  RosActionServer(ros::NodeHandle &node_handle, const std::string &server_name)
      : server_(node_handle, server_name, false), name_(server_name) {
    server_.start();
  }

  const std::string &name() const { return name_; }

  const GoalType<RosActionType> &goal() const { return *goal_; }

  bool is_new_goal_available() const { return server_.isNewGoalAvailable(); }

  bool is_goal_active() const { return server_.isActive(); }

  bool is_preempt_requested() const { return server_.isPreemptRequested(); }

  void accept_new_goal() {
    if (is_new_goal_available()) {
      goal_ = server_.acceptNewGoal();
    }
  }

  void publish_feedback(const FeedbackType<RosActionType> &feedback) const {
    server_.publishFeedback(feedback);
  }

  void set_succeeded(const ResultType<RosActionType> &result) {
    server_.setSucceeded(result, "");
  }

  void set_preempted(const ResultType<RosActionType> &result) {
    server_.setPreempted(result, "");
  }

  void set_aborted(const ResultType<RosActionType> &result) {
    server_.setAborted(result, "");
  }

private:
  std::string name_;
  mutable ServerType<RosActionType> server_;
  boost::shared_ptr<const GoalType<RosActionType>> goal_;
};

} // namespace evo::behavior
