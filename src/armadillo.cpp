#include "mantella_bits/armadillo.hpp"

// C++ standard library
#include <functional>

namespace mant {   
  arma::uword Hash::operator()(
      const arma::Col<double>& key) const {
    // Starts with the hash of the first value ...
    arma::uword hashedKey = static_cast<arma::uword>(std::hash<double>()(key(0)));

    // ... and adds the hash value of all following values to it.
    // Note: This is adapted from the Boost library (boost::hash_combine), including the magic numbers.
    for (const auto& value : key) {
      hashedKey ^= static_cast<arma::uword>(std::hash<double>()(value)) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
    }

    return hashedKey;
  }

  bool IsEqual::operator()(
      const arma::Col<double>& firstKey,
      const arma::Col<double>& secondKey) const {
    if (firstKey.n_elem != secondKey.n_elem) {
      return false;
    }

    return arma::all(firstKey == secondKey);
  }
}
