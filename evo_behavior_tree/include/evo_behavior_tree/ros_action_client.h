#pragma once

#include <actionlib/client/simple_action_client.h>
#include <ros/ros.h>

#include "ros_action_alias.h"

namespace evo::behavior {

template <class RosActionType> class RosActionClient {
public:
  explicit RosActionClient(const std::string &client_name)
      : client_(client_name, true), name_(client_name) {}

  const std::string &name() const { return name_; }

  bool is_active() const { return is_active_; }

  bool active_triggered() {
    bool is_triggered = active_triggered_;
    active_triggered_ = false;
    return is_triggered;
  }

  bool feedback_triggered() const { return feedback_triggered_; }

  bool done_triggered() {
    bool is_done = done_triggered_;
    done_triggered_ = false;
    return is_done;
  }

  const GoalType<RosActionType> &goal() const { return goal_; }

  const FeedbackType<RosActionType> &feedback() {
    feedback_triggered_ = false;
    return *feedback_;
  }

  const ResultType<RosActionType> &result() const { return *result_; }

  void send_goal(const GoalType<RosActionType> &goal) {
    reset();
    client_.sendGoal(
        goal, boost::bind(&RosActionClient::done_callback_, this, _1, _2),
        boost::bind(&RosActionClient::active_callback_, this),
        boost::bind(&RosActionClient::feedback_callback_, this, _1));
    goal_ = goal;
  }

  void cancel_goal() {
    client_.cancelGoal();
    goal_ = GoalType<RosActionType>();
  }

private:
  std::string name_;
  ClientType<RosActionType> client_;
  bool is_active_ = false;
  bool active_triggered_ = false;
  bool feedback_triggered_ = false;
  bool done_triggered_ = false;
  GoalType<RosActionType> goal_;
  boost::shared_ptr<const FeedbackType<RosActionType>> feedback_;
  boost::shared_ptr<const ResultType<RosActionType>> result_;

  void active_callback_() {
    done_triggered_ = false;
    is_active_ = true;
    active_triggered_ = true;
  }

  void feedback_callback_(
      const boost::shared_ptr<const FeedbackType<RosActionType>> &feedback) {
    feedback_ = feedback;
    feedback_triggered_ = true;
  }

  void done_callback_(
      [[maybe_unused]] const GoalStateType &state,
      const boost::shared_ptr<const ResultType<RosActionType>> &result) {
    result_ = result;
    is_active_ = false;
    done_triggered_ = true;
  }

  void reset() {
    is_active_ = false;
    active_triggered_ = false;
    feedback_triggered_ = false;
    done_triggered_ = false;
    goal_ = GoalType<RosActionType>();
    feedback_.reset();
    result_.reset();
  }
};

} // namespace evo::behavior
