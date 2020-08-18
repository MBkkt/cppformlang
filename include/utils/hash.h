#pragma once

#include <cstdint>
#include <functional>

namespace utils {

/// from https://www.boost.org/doc/libs/1_74_0/boost/container_hash/hash.hpp

#if defined(_MSC_VER)
#define BOOST_FUNCTIONAL_HASH_ROTL32(x, r) _rotl(x, r)
#else
#define BOOST_FUNCTIONAL_HASH_ROTL32(x, r) (x << r) | (x >> (32 - r))
#endif

inline void hash_combine_32(std::uint32_t& seed, std::uint32_t value) {
  static constexpr uint32_t c1 = 0xcc9e2d51U;
  static constexpr uint32_t c2 = 0x1b873593U;

  value *= c1;
  value = BOOST_FUNCTIONAL_HASH_ROTL32(value, 15);
  value *= c2;

  seed ^= value;
  seed = BOOST_FUNCTIONAL_HASH_ROTL32(seed, 13);
  seed = seed * 5 + 0xe6546b64;
}

inline void hash_combine_64(std::uint64_t& seed, std::uint64_t value) {
  static constexpr std::uint64_t m = 0xc6a4a7935bd1e995UL;
  static constexpr int r = 47;

  value *= m;
  value ^= value >> r;
  value *= m;

  seed ^= value;
  seed *= m;

  // Completely arbitrary number, to prevent 0's from hashing to 0.
  seed += 0xe6546b64;
}

template <typename Value, typename Iterator>
std::size_t hash_range(Iterator begin, Iterator end) {
  std::hash<Value> hasher;
  if constexpr (sizeof(std::size_t) == sizeof(std::uint64_t)) {
    std::uint64_t seed = 0;
    for (; begin != end; ++begin) {
      hash_combine_64(seed, hasher(*begin));
    }
    return seed;
  } else {
    std::uint32_t seed = 0;
    for (; begin != end; ++begin) {
      hash_combine_32(seed, hasher(*begin));
    }
    return seed;
  }
}

}  // namespace utils