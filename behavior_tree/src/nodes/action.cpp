#include <behavior_tree/nodes/action.h>
#include <iostream>

namespace evo::behavior {

Action::Action(std::function<void()> behavior, std::string const &description)
    : BehaviorNode("action", description), behavior_(behavior) {}

Status Action::operator()() {
  try {
    behavior_();
  } catch (const std::exception &e) {
    // In case of an exception, log it with the description of the action.
    std::cout << "Exception in behavior action '" << description()
              << "': " << e.what() << std::endl;
    return Status::Failure;
  } catch (...) {
    // Catch any other exceptions and log with the description of the action.
    std::cout << "Unknown exception in behavior action '" << description()
              << "'." << std::endl;
    return Status::Failure;
  }
  return Status::Success; // Return Success only if no exception is thrown
}

} // namespace evo::behavior
