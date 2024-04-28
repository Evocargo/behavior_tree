#pragma once

#include <ostream> // Include for ostream operator

namespace evo::behavior {

/**
 * @brief A class to represent status with three states: Success, Failure, and
 * Running.
 *
 */
class Status {
public:
  enum State { FAILURE, SUCCESS, RUNNING };

  Status(bool success);
  Status(State state);
  Status(const Status &other);
  Status(Status &&other) noexcept;
  Status &operator=(const Status &other);
  Status &operator=(Status &&other) noexcept;
  operator bool() const;
  operator State() const;

  bool operator==(const Status &other) const;

  bool operator!=(const Status &other) const;

  bool operator==(const State &state) const;
  bool operator!=(const State &state) const;
  static const Status Success;
  static const Status Failure;
  static const Status Running;

  friend std::ostream &operator<<(std::ostream &os, const Status &status);

private:
  State state_;
};

} // namespace evo::behavior
