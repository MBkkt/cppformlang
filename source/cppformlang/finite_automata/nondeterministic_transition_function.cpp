#include <cppformlang/finite_automata/nondeterministic_transition_function.h>
namespace cppformlang::finite_automata {

bool NondeterministicTransitions::AddTransition(State from, Symbol by,
                                                State to) {
  return transitions_[from][by].insert(to).second;
}

bool NondeterministicTransitions::RemoveTransition(State from, Symbol by,
                                                   State to) {
  auto it_from = transitions_.find(from);
  if (it_from == transitions_.end()) {
    return false;
  }
  auto it_by = it_from->second.find(by);
  if (it_by == it_from->second.end()) {
    return false;
  }
  auto it_to = it_by->second.find(to);
  if (it_to == it_by->second.end()) {
    return false;
  }
  it_by->second.erase(it_to);
  if (!it_by->second.empty()) {
    return true;
  }
  it_from->second.erase(it_by);
  if (!it_from->second.empty()) {
    return true;
  }
  transitions_.erase(it_from);
  return true;
}

size_t NondeterministicTransitions::TransitionsCount() const {
  std::size_t size = 0;
  for (auto& [_, from] : transitions_) {
    for (auto& [_, by] : from) {
      size += by.size();
    }
  }
  return size;
}

const std::unordered_set<State>* NondeterministicTransitions::operator()(
    State from, Symbol by) const {
  auto it_from = transitions_.find(from);
  if (it_from == transitions_.end()) {
    return nullptr;
  }
  auto it_by = it_from->second.find(by);
  if (it_by == it_from->second.end()) {
    return nullptr;
  }
  return &it_by->second;
}

bool NondeterministicTransitions::IsDeterministic() const {
  for (auto& [_, from] : transitions_) {
    for (auto& [_, by] : from) {
      if (by.size() > 1) {
        return false;
      }
    }
  }
  return true;
}

}  // namespace cppformlang::finite_automata
