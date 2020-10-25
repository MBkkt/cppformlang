#pragma once

#include <functional>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "finite_automata.h"

namespace cppformlang::finite_automata {

/**
 * \brief A nondeterministic transition function in a finite automaton.
 *
 * \details The difference between a deterministic transition is that the
 * initial state and the transition symbol can specify a set of states
 */
class NondeterministicTransitions {
 public:
  /**
   * \brief Whether the transition function is deterministic
   *
   * \param[in,out] f The functor
   *
   * \return Nothing
   */
  template <typename Functor>
  void ForeachTransition(Functor f) const {
    for (auto& [from, by_set] : transitions_) {
      for (auto& [by, to_set] : by_set) {
        for (auto& to : to_set) {
          f(from, by, to);
        }
      }
    }
  }

  /**
   * \brief Gives the number of transitions describe by the function
   *
   * \return The number of transitions
   */
  std::size_t TransitionsCount() const;

 protected:
  /**
   * \brief Adds a new transition to the function
   *
   * \param[in] from The source state
   * \param[in] by   The transition symbol
   * \param[in] to   The destination state
   *
   * \return true is the transition was not found and added, false otherwise
   */
  bool AddTransition(State from, Symbol by, State to);

  /**
   * \brief Removes a transition to the function
   *
   * \param[in] from The source state
   * \param[in] by   The transition symbol
   * \param[in] to   The destination state
   *
   * \return true is the transition was found and removed, false otherwise
   */
  bool RemoveTransition(State from, Symbol by, State to);

  /**
   * \brief Whether the transition function is deterministic
   *
   * \return Whether the function is deterministic
   */
  bool IsDeterministic() const;

  /**
   * \brief Removes a transition to the function
   *
   * \param[in] from The source state
   * \param[in] by   The transition symbol
   *
   * \return The destination states or nullptr if it does not exists
   */
  const std::unordered_set<State>* operator()(State from, Symbol by) const;

 private:
  std::unordered_map<State, std::unordered_map<Symbol, std::unordered_set<State>>> transitions_;
};

}  // namespace cppformlang::finite_automata
