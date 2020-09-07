#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "finite_automata.h"
#include "finite_automation.h"
#include "nondeterministic_transitions.h"

namespace cppformlang::finite_automata {

/**
 * \brief Represents a general finite automaton
 */
class NondeterministicFiniteAutomaton
    : public FiniteAutomaton<NondeterministicTransitions> {
 public:
  NondeterministicFiniteAutomaton();

  explicit NondeterministicFiniteAutomaton(char c);

  static NondeterministicFiniteAutomaton FromRegex(const std::string& regex);

  /**
   * \brief Makes the union of this and other object
   *
   * \param[in] The other object
   */

  void Union(const FiniteAutomaton& other);

  /**
   * \brief Makes the concatenation of this and other object
   *
   * \param[in] The other object
   */
  void Concatenate(const FiniteAutomaton& other);

  /**
   * \brief Makes the kleene star of the current object
   */
  void KleeneStar();

  /**
   * \brief Checks whether the nfa accepts a given word
   *
   * \param[in] begin Begin of the word(source symbols)
   * \param[in] end   End of the word(source symbols)
   *
   * \return Whether the word is accepted or not
   */
  bool Accepts(const Symbol* begin, const Symbol* end) const;

  /**
   * \brief Compute the epsilon closure of of state
   *
   * \param[in] state The source state
   *
   * \return The epsilon closure of the source state
   */
  std::unordered_set<State> Eclose(State state) const;

  bool IsDeterministic() const;

  NondeterministicFiniteAutomaton ToDeterministic() const;

  NondeterministicFiniteAutomaton Minimize() const;

 private:
  std::unordered_set<State> GetNextStates(
      const std::unordered_set<State>& current_states, Symbol symbol) const;
};

}  // namespace cppformlang::finite_automata