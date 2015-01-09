#pragma once

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

namespace mant {
  // Calculates a hash value for custom types.
  class Hasher {
    public:
      explicit Hasher() = default;

      explicit Hasher(const Hasher&) = default;

      Hasher& operator=(const Hasher&) = delete;

      // Returns a hash value for column vectors.
      // Note: This is adapted from the Boost library (boost::hash_combine).
      std::size_t operator() (
          const arma::Col<double>& key) const noexcept;
  };

  // Checks whether two keys of a custom type are equal.
  class KeyEqual {
    public:
      explicit KeyEqual() = default;

      KeyEqual(const KeyEqual&) = default;

      KeyEqual& operator=(const KeyEqual&) = delete;

      // Returns true if all values of both column vectors are equal.
      bool operator() (
          const arma::Col<double>& firstKey,
          const arma::Col<double>& secondKey) const noexcept;
  };
}
