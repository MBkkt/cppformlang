#include <cppformlang/finite_automata/recursive_automation.h>

namespace cppformlang::finite_automata {

bool RecursiveAutomaton::AddBox(Symbol label, NondeterministicFiniteAutomaton dfa) {
  if (auto it = boxes_.find(label); it != boxes_.end()) {
    it->second = std::move(dfa);
    return true;
  }
  boxes_.emplace_hint(boxes_.end(), label, std::move(dfa));
  return false;
}

const NondeterministicFiniteAutomaton* RecursiveAutomaton::GetBox(Symbol label) const {
  if (auto it = boxes_.find(label); it != boxes_.end()) {
    return &it->second;
  }
  return nullptr;
}

void RecursiveAutomaton::SetInitLabel(Symbol label) { init_label_ = label; }

}  // namespace cppformlang::finite_automata