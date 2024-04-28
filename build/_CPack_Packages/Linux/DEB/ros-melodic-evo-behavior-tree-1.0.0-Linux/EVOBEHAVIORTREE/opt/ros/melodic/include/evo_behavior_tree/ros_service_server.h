#pragma once

#include <ros/ros.h>

namespace evo::behavior {

/**
 * @brief
 *
 * @tparam RosServiceType
 */
template <class RosServiceType> struct RosServiceServer {
public:
  explicit RosServiceServer(typename RosServiceType::Request const &request)
      : request_(request), triggered_(true) {}

  RosServiceServer() : triggered_(false) {}

  bool triggered() const { return triggered_; }

  typename RosServiceType::Request const &request() { return request_; }
  void respond(typename RosServiceType::Response const &response) {
    response_ = response;
    responded_ = true;
  }

  bool responded(typename RosServiceType::Response &response) {
    response = response_;
    return responded_;
  }

  void clear_after_callback() {
    request_ = typename RosServiceType::Request();
    response_ = typename RosServiceType::Response();
    triggered_ = false;
    responded_ = false;
  }

private:
  typename RosServiceType::Request request_;
  typename RosServiceType::Response response_;
  bool triggered_;
  bool responded_ = false;
};

} // namespace evo::behavior
