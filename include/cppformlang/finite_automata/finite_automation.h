#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "finite_automata.h"

namespace cppformlang::finite_automata {

/**
 * \brief Represents a general finite automaton
 */
template <typename Transitions>
class FiniteAutomaton : public Transitions {
 public:
  /**
   * \brief Adds a transition to the finite automata
   *
   * \param[in] from The source state
   * \param[in] by   The transition symbol
   * \param[in] to   The destination state
   *
   * \return true is the transition was added, false otherwise(was founded)
   */
  bool AddTransition(State from, Symbol by, State to);

  /**
   * \brief Remove a transition of the finite automata
   *
   * \param[in] from The source state
   * \param[in] by   The transition symbol
   * \param[in] to   The destination state
   *
   * \return true is the transition was removed, false otherwise(was not
   * founded)
   */
  bool RemoveTransition(State from, Symbol by, State to);

  bool SetStartState(State state, bool is_start);
  bool SetFinalState(State state, bool is_final);

  bool IsStateStart(State state) const;
  bool IsStateFinal(State state) const;
  bool IsAcyclic() const;

  std::size_t StatesCount() const;
  std::size_t StartStatesCount() const;
  std::size_t FinalStatesCount() const;
  std::size_t SymbolsCount() const;

  template <typename Functor>
  void ForeachState(Functor f) const {
    for (auto& [state, _] : states_) {
      f(state);
    }
  }
  template <typename Functor>
  void ForeachStartState(Functor f) const {
    for (auto& state : start_states_) {
      f(state);
    }
  }

  template <typename Functor>
  void ForeachFinalState(Functor f) const {
    for (auto& state : final_states_) {
      f(state);
    }
  }

  template <typename Functor>
  void ForeachSymbol(Functor f) const {
    for (auto& [symbol, _] : symbols_) {
      f(symbol);
    }
  }

 protected:
  std::unordered_map<State, std::uint32_t> states_;
  std::unordered_set<State> start_states_;
  std::unordered_set<State> final_states_;
  std::unordered_map<Symbol, std::uint32_t> symbols_;
};

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

}  // namespace cppformlang::finite_automata