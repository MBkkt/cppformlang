#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "finite_automata.h"
#include "finite_automation.h"

namespace cppformlang::finite_automata {

/**
 * \brief Represents a general finite automaton
 */
class NondeterministicFiniteAutomaton : public FiniteAutomaton {
 public:
  // void Accepts(const Symbol* begin, const Symbol* end) const;
  std::unordered_set<State> Eclose(State state) const;
  bool IsDeterministic() const;
  NondeterministicFiniteAutomaton ToDeterministic() const;
};

}  // namespace cppformlang::finite_automata