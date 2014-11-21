#include <hop_bits/helper/cache.hpp>

// C++ Standard Library
#include <functional>

namespace hop {
  std::size_t CacheHasher::operator() (
    const arma::Col<double>& key) const {
    // Start with the hash of the first value ...
    std::size_t hashedKey = std::hash<double>()(key.at(0));

    // ... and add the hash value of all following values to it.
    for (const double& value : key) {
      hashedKey ^= std::hash<double>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
    }

    return hashedKey;
  }

  bool CacheKeyEqual::operator() (
    const arma::Col<double>& firstKey,
    const arma::Col<double>& secondKey) const {
    // This will also check if both vectors have the same size.
    return arma::all(firstKey == secondKey);
  }
}
