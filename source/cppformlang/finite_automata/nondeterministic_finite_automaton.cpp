#include <cppformlang/finite_automata/nondeterministic_finite_automaton.h>
#include <utils/hash.h>

#include <cassert>
#include <stack>

namespace cppformlang::finite_automata {

bool NondeterministicFiniteAutomaton::IsDeterministic() const {
  if (start_states_.size() > 1 || !transition_functions_.IsDeterministic()) {
    return false;
  }
  for (auto& [state, counter] : states_) {
    if (Eclose(state).size() > 1) {
      return false;
    }
  }
  return true;
}

std::unordered_set<State> NondeterministicFiniteAutomaton::Eclose(
    State state) const {
  std::unordered_set<State> processed{state};
  std::stack<State> to_process;
  to_process.push(state);
  while (!to_process.empty()) {
    auto current = to_process.top();
    to_process.pop();
    if (auto connected = transition_functions_(current, kEpsilon)) {
      for (auto& conn_state : *connected) {
        if (auto it = processed.find(conn_state); it == processed.end()) {
          processed.insert(it, state);
          to_process.push(state);
        }
      }
    }
  }
  return processed;
}

struct HashStates {
  std::size_t operator()(const std::unordered_set<State>& value) const {
    return utils::hash_range<State>(value.begin(), value.end());
  }
};

NondeterministicFiniteAutomaton
NondeterministicFiniteAutomaton::ToDeterministic() const {
  NondeterministicFiniteAutomaton dfa;
  if (start_states_.empty()) {
    assert(states_.empty());
    return dfa;
  }
  State state = 0;

  std::unordered_set<State> current = Eclose(*start_states_.begin());
  std::unordered_set<State> next;

  std::unordered_map<std::unordered_set<State>, State, HashStates> processed;
  processed.emplace(current, state);
  std::stack<std::tuple<std::unordered_set<State>, State>> to_process;
  to_process.emplace(current, state);

  while (!to_process.empty()) {
    auto [current, from_state] = to_process.top();
    to_process.pop();
    for (auto& [symbol, _] : symbols_) {
      for (auto from : current) {
        if (auto to_set = transition_functions_(from, symbol)) {
          next.insert(to_set->begin(), to_set->end());
        }
      }
      bool is_final = false;
      for (auto to : next) {
        for (auto to_close : Eclose(to)) {
          next.insert(to_close);
          if (!is_final && IsStateFinal(to_close)) {
            is_final = true;
          }
        }
      }
      if (next.empty()) {
        continue;
      }
      State to_state;
      if (auto it_to = processed.find(next); it_to != processed.end()) {
        to_state = it_to->second;
      } else {
        processed.emplace_hint(it_to, next, from_state);
        to_state = ++state;
        to_process.emplace(next, to_state);
      }
      dfa.AddTransition(from_state, symbol, to_state);
      if (is_final) {
        dfa.SetState(to_state, false, is_final);
      }
    }
  }
  dfa.SetState(0, true, false);
  return dfa;
}

}  // namespace cppformlang::finite_automata
