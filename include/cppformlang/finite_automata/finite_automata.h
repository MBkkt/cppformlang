#pragma once

#include <cstdint>
#include <limits>

namespace cppformlang::finite_automata {

using Symbol = std::int32_t;
using State = std::uint32_t;

inline constexpr Symbol kEpsilon = std::numeric_limits<Symbol>::min();

}  // namespace cppformlang::finite_automata
