#include <cppformlang/finite_automata/nondeterministic_finite_automaton.h>
#include <doctest/doctest.h>

#include <string>

using namespace cppformlang::finite_automata;

TEST_CASE("ToDeterministic1") {
  NondeterministicFiniteAutomaton nfa;
  auto state0 = State{10};
  auto state1 = State{11};
  auto state2 = State{12};
  auto symbol0 = Symbol{0};
  auto symbol1 = Symbol{1};
  nfa.AddTransition(state0, symbol0, state0);
  nfa.AddTransition(state0, symbol0, state1);
  nfa.AddTransition(state0, symbol1, state0);
  nfa.AddTransition(state1, symbol1, state2);
  nfa.SetState(state0, true, false);
  nfa.SetState(state1, false, true);
  auto dfa = nfa.ToDeterministic();
  CHECK(dfa.IsDeterministic());
  CHECK(dfa.StateCount() == 3);
  CHECK(dfa.TransitionCount() == 6);
}

TEST_CASE("ToDeterministic2") {
  NondeterministicFiniteAutomaton nfa;
  auto state0 = State{10};
  auto state1 = State{11};
  auto symbol0 = Symbol{0};
  auto symbol1 = Symbol{1};
  nfa.AddTransition(state0, symbol0, state0);
  nfa.AddTransition(state0, symbol1, state0);
  nfa.AddTransition(state0, symbol0, state1);
  nfa.AddTransition(state1, symbol1, state0);
  nfa.AddTransition(state0, symbol1, state0);
  nfa.AddTransition(state1, symbol1, state1);

  nfa.SetState(state0, true, false);
  nfa.SetState(state1, false, true);
  auto dfa = nfa.ToDeterministic();
  CHECK(dfa.IsDeterministic());
  CHECK(dfa.StateCount() == 2);
  CHECK(dfa.TransitionCount() == 4);
}
