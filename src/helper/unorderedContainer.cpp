#include "mantella_bits/helper/unorderedContainer.hpp"

// C++ standard library
#include <functional>

namespace mant {
  arma::uword Hash::operator()(
      const arma::Col<double>& key) const {
    // Start with the hash of the first value ...
    arma::uword hashedKey = std::hash<double>()(key(0));

    // ... and add the hash value of all following values to it.
    // Note: This is adapted from the Boost library (boost::hash_combine).
    for (const auto& value : key) {
      hashedKey ^= std::hash<double>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
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
