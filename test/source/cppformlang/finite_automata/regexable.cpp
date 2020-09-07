#include <cppformlang/finite_automata/nondeterministic_finite_automaton.h>
#include <doctest/doctest.h>

#include <string>

using namespace cppformlang::finite_automata;

TEST_CASE("FromRegex") {
  auto regex = R"(a|b+c*)";
  auto nfa = NondeterministicFiniteAutomaton::FromRegex(regex);
  for (auto i = 0; i != 2; ++i) {
    const Symbol check1[] = {'a'};
    CHECK(nfa.Accepts(std::begin(check1), std::end(check1)));
    const Symbol check2[] = {'b'};
    CHECK(nfa.Accepts(std::begin(check2), std::end(check2)));
    const Symbol check3[] = {'b', 'c'};
    CHECK(nfa.Accepts(std::begin(check3), std::end(check3)));
    const Symbol check4[] = {'b', 'c', 'c'};
    CHECK(nfa.Accepts(std::begin(check4), std::end(check4)));
    nfa = nfa.ToDeterministic();
  }
}