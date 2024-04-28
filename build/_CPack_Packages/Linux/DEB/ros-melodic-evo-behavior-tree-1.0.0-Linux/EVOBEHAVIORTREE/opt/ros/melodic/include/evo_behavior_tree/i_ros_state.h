#pragma once

#include <ros/ros.h>

#include "behavior_tree/bt_base.h"
#include "ros_action_client.h"
#include "ros_action_server.h"
#include "ros_service_server.h"

namespace evo::behavior {

class IRosState : public BehaviorTree {
public:
  IRosState(BehaviorPtr root, ros::NodeHandle &nh, ros::NodeHandle &pnh)
      : BehaviorTree(root), nh_(nh), pnh_(pnh) {}

  IRosState(ros::NodeHandle &nh, ros::NodeHandle &pnh)
      : IRosState(nullptr, nh, pnh) {}

  void set_root(BehaviorPtr root) override { BehaviorTree::set_root(root); }

  void spin(size_t rate_hz) {
    ros::Rate rate(static_cast<double>(rate_hz));
    while (ros::ok()) {
      time_stamp = ros::Time::now();
      time = time_stamp.toSec();
      run();
      rate.sleep();
      ros::spinOnce();
    }
  }

  virtual ~IRosState() = default;

protected:
  ros::NodeHandle &nh_;
  ros::NodeHandle &pnh_;

  double time;
  ros::Time time_stamp;

  template <class RosActionType>
  void action_server(std::string const &server_name,
                     std::unique_ptr<RosActionServer<RosActionType>> &server,
                     ros::NodeHandle &nh) const {
    server = std::make_unique<RosActionServer<RosActionType>>(nh, server_name);
  }

  template <class RosActionType>
  void
  action_client(std::string const &client_name,
                std::unique_ptr<RosActionClient<RosActionType>> &client) const {
    client = std::make_unique<RosActionClient<RosActionType>>(client_name);
  }

  template <class RosServiceType>
  void service_server(std::string const &service_name,
                      RosServiceServer<RosServiceType> &ros_service_server,
                      ros::NodeHandle &nh) {
    boost::function<bool(typename RosServiceType::Request &,
                         typename RosServiceType::Response &)>
        callback =
            make_service_server_callback_(ros_service_server, service_name);
    ros_service_servers_.push_back(nh.advertiseService(service_name, callback));
  }

  template <class RosServiceType>
  void service_server(std::string const &service_name,
                      RosServiceServer<RosServiceType> &ros_service_server) {
    boost::function<bool(typename RosServiceType::Request &,
                         typename RosServiceType::Response &)>
        callback =
            make_service_server_callback_(ros_service_server, service_name);
    ros_service_servers_.push_back(
        nh_.advertiseService(service_name, callback));
  }

  template <class RosMessageType>
  void subscriber(std::string const &topic_name, uint32_t queue_length,
                  RosMessageType &msg, ros::NodeHandle &nh) {
    boost::function<void(typename RosMessageType::ConstPtr const &)> callback =
        make_ros_message_callback_(msg, topic_name);
    ros_subscribers_.push_back(
        nh.subscribe(topic_name, queue_length, callback));
  }

  template <class RosMessageType>
  void subscriber(std::string const &topic_name, uint32_t queue_length,
                  RosMessageType &msg) {
    boost::function<void(typename RosMessageType::ConstPtr const &)> callback =
        make_ros_message_callback_(msg, topic_name);
    ros_subscribers_.push_back(
        nh_.subscribe(topic_name, queue_length, callback));
  }

  template <class RosMessageType>
  void publisher(std::string const &topic_name, uint32_t queue_length,
                 ros::Publisher &pub, ros::NodeHandle &nh, bool latch = false) {
    pub = nh.advertise<RosMessageType>(topic_name, queue_length, latch);
  }

  template <class RosMessageType>
  void publisher(std::string const &topic_name, uint32_t queue_length,
                 ros::Publisher &pub, bool latch = false) {
    pub = nh_.advertise<RosMessageType>(topic_name, queue_length, latch);
  }

  template <class RosServiceType>
  void service_client(std::string const &service_name,
                      ros::ServiceClient &ros_service_client) {
    ros_service_client = nh_.serviceClient<RosServiceType>(service_name);
  }

private:
  std::vector<ros::ServiceServer> ros_service_servers_;
  std::vector<ros::Subscriber> ros_subscribers_;

  template <class RosServiceType>
  std::function<bool(typename RosServiceType::Request &,
                     typename RosServiceType::Response &)>
  make_service_server_callback_(RosServiceServer<RosServiceType> &server,
                                const std::string &service_name) {
    return [&, service_name](typename RosServiceType::Request &request,
                             typename RosServiceType::Response &response) {
      server = RosServiceServer<RosServiceType>(request);
      run();
      return server.responded(response);
    };
  }

  template <class RosMessageType>
  std::function<void(typename RosMessageType::ConstPtr const &)>
  make_ros_message_callback_(RosMessageType &msg,
                             const std::string &topic_name) {
    return [&, topic_name](typename RosMessageType::ConstPtr const &message) {
      msg = *message;
      run();
      return;
    };
  }
};

} // namespace evo::behavior
