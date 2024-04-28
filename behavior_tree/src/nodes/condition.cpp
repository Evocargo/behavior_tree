#include <behavior_tree/nodes/condition.h>
#include <iostream>

namespace evo::behavior {

Condition::Condition(std::function<Status()> condition,
                     const std::string &description)
    : BehaviorNode("condition", description), condition_(condition) {}

Status Condition::operator()() {
  try {
    return condition_();
  } catch (const std::exception &e) {
    std::cerr << "Exception in behavior condition '" << description()
              << "': " << e.what() << std::endl;
    return Status::Failure;
  } catch (...) {
    std::cerr << "Unknown exception in behavior condition '" << description()
              << "'." << std::endl;
    return Status::Failure;
  }
}

} // namespace evo::behavior
