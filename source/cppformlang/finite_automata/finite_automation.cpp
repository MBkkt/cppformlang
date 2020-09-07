#include <cppformlang/finite_automata/finite_automation.h>
#include <cppformlang/finite_automata/nondeterministic_transitions.h>

namespace cppformlang::finite_automata {

template <typename Transitions>
bool FiniteAutomaton<Transitions>::AddTransition(State from, Symbol by,
                                                 State to) {
  if (!Transitions::AddTransition(from, by, to)) {
    return false;
  }
  ++states_[from];
  ++states_[to];
  ++symbols_[by];
  return true;
}
template <typename Transitions>
bool FiniteAutomaton<Transitions>::RemoveTransition(State from, Symbol by,
                                                    State to) {
  if (!Transitions::RemoveTransition(from, by, to)) {
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
  if (auto it_by = symbols_.find(by); --it_by->second == 0) {
    symbols_.erase(it_by);
  }
  return true;
}

template <typename Transitions>
std::size_t FiniteAutomaton<Transitions>::StatesCount() const {
  return states_.size();
}

template <typename Transitions>
std::size_t FiniteAutomaton<Transitions>::StartStatesCount() const {
  return start_states_.size();
}

template <typename Transitions>
std::size_t FiniteAutomaton<Transitions>::FinalStatesCount() const {
  return final_states_.size();
}

template <typename Transitions>
std::size_t FiniteAutomaton<Transitions>::SymbolsCount() const {
  return symbols_.size();
}

template <typename Transitions>
bool FiniteAutomaton<Transitions>::SetStartState(State state, bool is_start) {
  auto it = states_.find(state);
  if (it == states_.end()) {
    return false;
  }
  if (is_start) {
    start_states_.insert(state);
  } else {
    start_states_.erase(state);
  }
  return true;
}

template <typename Transitions>
bool FiniteAutomaton<Transitions>::SetFinalState(State state, bool is_final) {
  auto it = states_.find(state);
  if (it == states_.end()) {
    return false;
  }
  if (is_final) {
    final_states_.insert(state);
  } else {
    final_states_.erase(state);
  }
  return true;
}

template <typename Transitions>
bool FiniteAutomaton<Transitions>::IsStateStart(State state) const {
  return start_states_.find(state) != start_states_.end();
}

template <typename Transitions>
bool FiniteAutomaton<Transitions>::IsStateFinal(State state) const {
  return final_states_.find(state) != final_states_.end();
}

template <typename Transitions>
bool FiniteAutomaton<Transitions>::IsAcyclic() const {
  return false;
}

template class FiniteAutomaton<NondeterministicTransitions>;

}  // namespace cppformlang::finite_automata
