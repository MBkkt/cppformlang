#include <cppformlang/finite_automata/recursive_automation.h>
#include <doctest/doctest.h>

#include <string>

namespace {

using namespace cppformlang::finite_automata;

TEST_CASE("AddBox") {
  RecursiveAutomaton rsa;
  auto nfa = NondeterministicFiniteAutomaton::FromRegex("a*+b*").Minimize();
  Symbol s = 'S';
  rsa.SetInitLabel(s);
  rsa.AddBox(s, nfa);
  // REQUIRE(rsa.GetBox(s) == nfa);
}

}  // namespace
