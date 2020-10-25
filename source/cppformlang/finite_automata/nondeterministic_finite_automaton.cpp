#include <cppformlang/finite_automata/nondeterministic_finite_automaton.h>
#include <utils/hash.h>

#include <algorithm>
#include <cassert>
#include <set>
#include <stack>
#include <string>

namespace cppformlang::finite_automata {

bool NondeterministicFiniteAutomaton::IsDeterministic() const {
  if (start_states_.size() > 1 || !FiniteAutomaton::IsDeterministic()) {
    return false;
  }
  return std::all_of(states_.begin(), states_.end(),
                     [this](const auto& state) { return Eclose(state.first).size() <= 1; });
}

std::unordered_set<State> NondeterministicFiniteAutomaton::Eclose(State state) const {
  std::unordered_set<State> processed{state};
  std::stack<State> to_process;
  to_process.push(state);
  while (!to_process.empty()) {
    auto current = to_process.top();
    to_process.pop();
    if (auto connected = (*this)(current, kEpsilon)) {
      for (auto& conn_state : *connected) {
        if (auto it = processed.find(conn_state); it == processed.end()) {
          processed.insert(it, conn_state);
          to_process.push(conn_state);
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

NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::ToDeterministic() const {
  NondeterministicFiniteAutomaton dfa;
  if (start_states_.empty()) {
    assert(states_.empty());
    return dfa;
  }
  State state = 1;

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
        if (auto to_set = (*this)(from, symbol)) {
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
        dfa.SetFinalState(to_state, is_final);
      }
    }
  }
  dfa.SetStartState(1, true);
  return dfa;
}
bool NondeterministicFiniteAutomaton::Accepts(const Symbol* begin, const Symbol* end) const {
  std::unordered_set<State> current_states;
  for (auto start : start_states_) {
    auto start_set = Eclose(start);
    current_states.insert(start_set.begin(), start_set.end());
  }
  for (; begin != end; ++begin) {
    if (*begin == kEpsilon) {
      continue;
    }
    auto next = GetNextStates(current_states, *begin);
    current_states.clear();
    for (auto start : next) {
      auto start_set = Eclose(start);
      current_states.insert(start_set.begin(), start_set.end());
    }
  }
  return std::any_of(current_states.begin(), current_states.end(),
                     [this](const auto& state) { return final_states_.find(state) != final_states_.end(); });
}

std::unordered_set<State> NondeterministicFiniteAutomaton::GetNextStates(
    const std::unordered_set<State>& current_states, Symbol symbol) const {
  std::unordered_set<State> next_states;
  for (auto current_state : current_states) {
    if (auto next_states_temp = (*this)(current_state, symbol)) {
      next_states.insert(next_states_temp->begin(), next_states_temp->end());
    }
  }
  return next_states;
}

NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::Minimize() const {
  auto dfa = ToDeterministic();
  // auto reachable_states = GetReachableStates();
  // std::unordered_set<State> states;
  // std::set_intersection(states_.begin(), states_.end(),
  //                       reachable_states.begin(), reachable_states.end(),
  //                       states.begin());
  // auto partition = GetPartition();
  // auto groups = GetGroups();
  // std::unordered_map<State, State> to_new_states;
  // State new_state = 0;
  // for (const auto& group : groups) {
  //   for (auto state : group) {
  //     to_new_states[state] = new_state;
  //   }
  //   ++new_state;
  // }
  // for (auto state : start_states_) {
  //   dfa.SetStartState(to_new_states[state], true);
  // }
  // for (auto state : start_states_) {
  //   dfa.SetFinalState(to_new_states[state], true);
  // }
  return dfa;
}

void NondeterministicFiniteAutomaton::Union(const FiniteAutomaton& other) {
  State max_state_number = 0;
  ForeachState([&](const auto& state) {
    if (max_state_number < state) {
      max_state_number = state;
    }
  });

  other.ForeachTransition([&](const auto& other_from, const auto& by, const auto& other_to) {
    AddTransition(other_from + max_state_number, by, other_to + max_state_number);
  });

  start_states_.reserve(start_states_.size() + other.StartStatesCount());
  other.ForeachStartState([&](const auto& state) { start_states_.insert(state + max_state_number); });
  final_states_.reserve(final_states_.size() + other.FinalStatesCount());
  other.ForeachFinalState([&](const auto& state) { final_states_.insert(state + max_state_number); });
}

void NondeterministicFiniteAutomaton::Concatenate(const FiniteAutomaton& other) {
  State max_state_number = 0;
  ForeachState([&](const auto& state) {
    if (max_state_number < state) {
      max_state_number = state;
    }
  });

  other.ForeachTransition([&](const auto& other_from, const auto& by, const auto& other_to) {
    AddTransition(other_from + max_state_number, by, other_to + max_state_number);
  });

  other.ForeachStartState([&](const auto& other_to) {
    auto to = other_to + max_state_number;
    ForeachFinalState([&](const auto& from) { AddTransition(from, kEpsilon, to); });
  });

  final_states_.clear();
  final_states_.reserve(other.FinalStatesCount());
  other.ForeachFinalState([&](const auto& state) { final_states_.insert(state + max_state_number); });
}

void NondeterministicFiniteAutomaton::KleeneStar() {
  ForeachFinalState(
      [this](const auto& from) { ForeachStartState([&](const auto& to) { AddTransition(from, kEpsilon, to); }); });
  final_states_.insert(start_states_.begin(), start_states_.end());
}

static std::string ToPostfix(const std::string& regex) {
  std::string answer;
  std::deque<char> operators;
  bool ignore = false;
  for (auto begin = regex.begin(), end = regex.end(); begin != end; ++begin) {
    if (ignore) {
      answer.push_back('\\');
      answer.push_back(*begin);
      ignore = false;
      continue;
    }
    switch (*begin) {
      case '\\': {
        ignore = true;
      } break;
      case '(': {
        operators.push_back('(');
      } break;
      case ')': {
        while (operators.back() != '(') {
          answer.push_back(operators.back());
          operators.pop_back();
        }
        operators.pop_back();
      } break;
      case '*': {
        answer.push_back('*');
      } break;
      case '|': {
        while (!operators.empty() && operators.back() == '+') {
          answer.push_back('+');
          operators.pop_back();
        }
        operators.push_back('|');
      } break;
      case '+': {
        operators.push_back('+');
      } break;
      default: {
        answer.push_back(*begin);
      } break;
    }
  }
  return answer + std::string{operators.rbegin(), operators.rend()};
}

NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::FromRegex(const std::string& regex) {
  auto postfix_regex = ToPostfix(regex);
  std::stack<NondeterministicFiniteAutomaton> operands;
  bool ignore = false;
  for (auto begin = postfix_regex.begin(), end = postfix_regex.end(); begin != end; ++begin) {
    if (ignore) {
      operands.push(NondeterministicFiniteAutomaton{*begin});
      ignore = false;
      continue;
    }
    switch (*begin) {
      case '\\': {
        ignore = true;
      } break;
      case '*': {
        operands.top().KleeneStar();
      } break;
      case '+': {
        auto right = operands.top();
        operands.pop();
        operands.top().Concatenate(right);
      } break;
      case '|': {
        auto right = operands.top();
        operands.pop();
        operands.top().Union(right);
      } break;
      default: {
        operands.push(NondeterministicFiniteAutomaton{*begin});
      } break;
    }
  }
  if (operands.empty()) {
    return {};
  }
  return operands.top();
}

NondeterministicFiniteAutomaton::NondeterministicFiniteAutomaton(char c) {
  AddTransition(1, c, 2);
  start_states_.insert(1);
  final_states_.insert(2);
}

NondeterministicFiniteAutomaton::NondeterministicFiniteAutomaton() = default;

}  // namespace cppformlang::finite_automata
