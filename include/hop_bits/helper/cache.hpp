#pragma once

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

namespace hop {
  // Calculates a hash value for custom types.
  class CacheHasher {
    public:
      // Returns a hash value for column vectors.
      // Note: This is adapted from the Boost library (boost::hash_combine).
      std::size_t operator() (
          const arma::Col<double>& key) const;
  };

  // Checks whether two keys of a custom type are equal.
  class CacheKeyEqual {
    public:
      // Returns true if all values of both column vectors are equal.
      bool operator() (
          const arma::Col<double>& firstKey,
          const arma::Col<double>& secondKey) const;
  };
}
