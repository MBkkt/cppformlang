#include <cppformlang/finite_automata/finite_automation.h>

namespace cppformlang::finite_automata {

bool FiniteAutomaton::AddTransition(State from, Symbol by, State to) {
  if (!transition_functions_.AddTransition(from, by, to)) {
    return false;
  }
  ++states_[from];
  ++states_[to];
  ++symbols_[by];
  return true;
}

bool FiniteAutomaton::RemoveTransition(State from, Symbol by, State to) {
  if (!transition_functions_.RemoveTransition(from, by, to)) {
    return false;
  }
  if (auto it_from = states_.find(from); --it_from->second == 0) {
    start_states_.erase(from);
    final_states_.erase(from);
    states_.erase(it_from);
  }
  if (auto it_to = states_.find(to); --it_to->second == 0) {
    start_states_.erase(to);
    final_states_.erase(to);
    states_.erase(it_to);
  }
  if (auto it_by = symbols_.find(by); --it_by->second <= 2) {
    symbols_.erase(it_by);
  }
  return true;
}

std::size_t FiniteAutomaton::StateCount() const { return states_.size(); }
std::size_t FiniteAutomaton::StartStateCount() const {
  return start_states_.size();
}
std::size_t FiniteAutomaton::FinalStateCount() const {
  return final_states_.size();
}
std::size_t FiniteAutomaton::SymbolCount() const { return symbols_.size(); }
std::size_t FiniteAutomaton::TransitionCount() const {
  return transition_functions_.Size();
}

bool FiniteAutomaton::SetState(State state, bool is_start, bool is_final) {
  auto it = states_.find(state);
  if (it == states_.end()) {
    return false;
  }
  if (is_start) {
    start_states_.insert(state);
  } else {
    start_states_.erase(state);
  }
  if (is_final) {
    final_states_.insert(state);
  } else {
    final_states_.erase(state);
  }
  return true;
}

bool FiniteAutomaton::IsStateStart(State state) const {
  return start_states_.find(state) != start_states_.end();
}

bool FiniteAutomaton::IsStateFinal(State state) const {
  return final_states_.find(state) != final_states_.end();
}

bool FiniteAutomaton::IsAcyclic() const { return false; }

}  // namespace cppformlang::finite_automata
