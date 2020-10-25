#pragma once

#include <unordered_map>

#include "finite_automata.h"
#include "nondeterministic_finite_automaton.h"

namespace cppformlang::finite_automata {

class RecursiveAutomaton {
 public:
  void SetInitLabel(Symbol label);
  bool AddBox(Symbol label, NondeterministicFiniteAutomaton dfa);
  const NondeterministicFiniteAutomaton* GetBox(Symbol label) const;

 private:
  std::unordered_map<Symbol, NondeterministicFiniteAutomaton> boxes_;
  Symbol init_label_;
};

}  // namespace cppformlang::finite_automata