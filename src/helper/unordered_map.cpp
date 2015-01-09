#include <mantella_bits/helper/unordered_map.hpp>

// C++ Standard Library
#include <functional>

namespace mant {
  std::size_t Hasher::operator() (
    const arma::Col<double>& key) const noexcept {
    // Start with the hash of the first value ...
    std::size_t hashedKey = std::hash<double>()(key.at(0));

    // ... and add the hash value of all following values to it.
    for (const double& value : key) {
      hashedKey ^= std::hash<double>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
    }

    return hashedKey;
  }

  bool KeyEqual::operator() (
    const arma::Col<double>& firstKey,
    const arma::Col<double>& secondKey) const noexcept {
    // This will also check if both vectors have the same size.
    return arma::all(firstKey == secondKey);
  }
}
