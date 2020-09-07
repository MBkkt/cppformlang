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

}  // namespace cppformlang::finite_automata