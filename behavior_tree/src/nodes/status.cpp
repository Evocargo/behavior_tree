#include "behavior_tree/nodes/status.h"
#include <iostream>

namespace evo::behavior {

Status::Status(bool success) : state_(success ? SUCCESS : FAILURE) {
  // std::cout << "Constructed Status with bool: " << success << " resulting
  // state: " << state_ << std::endl;
}

Status::Status(State state) : state_(state) {
  // std::cout << "Constructed Status with State: " << state << std::endl;
}

Status::Status(const Status &other) : state_(other.state_) {
  // std::cout << "Copy constructed Status with state: " << other.state_ <<
  // std::endl;
}

Status::Status(Status &&other) noexcept : state_(other.state_) {
  // std::cout << "Move constructed Status with state: " << other.state_ <<
  // std::endl;
}

Status &Status::operator=(const Status &other) {
  if (this != &other) {
    state_ = other.state_;
    // std::cout << "Copy assigned Status from state: " << other.state_ << " to
    // state: " << state_ << std::endl;
  }
  return *this;
}

Status &Status::operator=(Status &&other) noexcept {
  if (this != &other) {
    state_ = other.state_;
    // std::cout << "Move assigned Status from state: " << other.state_ << " to
    // state: " << state_ << std::endl;
  }
  return *this;
}

Status::operator bool() const { return state_ == SUCCESS; }
Status::operator State() const { return state_; }

bool Status::operator==(const State &state) const { return state_ == state; }
bool Status::operator!=(const State &state) const { return state_ != state; }

bool Status::operator==(const Status &other) const {
  return state_ == other.state_;
}

bool Status::operator!=(const Status &other) const {
  return state_ != other.state_;
}
std::ostream &operator<<(std::ostream &os, const Status &status) {
  switch (status.state_) {
  case Status::FAILURE:
    os << "FAILURE";
    break;
  case Status::SUCCESS:
    os << "SUCCESS";
    break;
  case Status::RUNNING:
    os << "RUNNING";
    break;
  default:
    os << "UNKNOWN";
  }
  return os;
}

// Definition of static members
const Status Status::Success(Status::SUCCESS);
const Status Status::Failure(Status::FAILURE);
const Status Status::Running(Status::RUNNING);

} // namespace evo::behavior
