#pragma once

#include <actionlib/client/simple_action_client.h>
#include <actionlib/server/simple_action_server.h>

namespace evo::behavior {

template <typename RosActionType>
using ClientType = actionlib::SimpleActionClient<RosActionType>;

template <typename RosActionType>
using ServerType = actionlib::SimpleActionServer<RosActionType>;

template <typename RosActionType>
using GoalType = typename RosActionType::_action_goal_type::_goal_type;

template <typename RosActionType>
using FeedbackType =
    typename RosActionType::_action_feedback_type::_feedback_type;

template <typename RosActionType>
using ResultType = typename RosActionType::_action_result_type::_result_type;

using GoalStateType = actionlib::SimpleClientGoalState;

} // namespace evo::behavior
