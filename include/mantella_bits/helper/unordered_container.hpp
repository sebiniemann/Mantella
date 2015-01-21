#pragma once

// C++ Standard Library
#include <cstdlib>
#include <functional>

// Armadillo
#include <armadillo>

namespace mant {
  // Calculates a hash value for custom types.
  class Hash {
    public:
      explicit Hash() = default;

      Hash(const Hash&) = default;

      Hash& operator=(const Hash&) = delete;

      inline std::size_t operator() (
          const arma::Col<double>& key) const ;

      inline std::size_t operator() (
          const arma::Col<unsigned int>& key) const ;
    };

  // Checks whether two keys of a custom type are equal.
  class IsKeyEqual {
    public:
      explicit IsKeyEqual() = default;

      IsKeyEqual(const IsKeyEqual&) = default;

      IsKeyEqual& operator=(const IsKeyEqual&) = delete;

      inline bool operator() (
          const arma::Col<double>& firstKey,
          const arma::Col<double>& secondKey) const ;

      inline bool operator() (
          const arma::Col<unsigned int>& firstKey,
          const arma::Col<unsigned int>& secondKey) const ;
  };

  //
  // Implementation
  //

  inline std::size_t Hash::operator() (
    const arma::Col<double>& key) const  {
    // Start with the hash of the first value ...
    std::size_t hashedKey = std::hash<double>()(key.at(0));

    // ... and add the hash value of all following values to it.
    // Note: This is adapted from the Boost library (boost::hash_combine).
    for (const double& value : key) {
      hashedKey ^= std::hash<double>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
    }

    return hashedKey;
  }

  inline std::size_t Hash::operator() (
    const arma::Col<unsigned int>& key) const  {
    // Start with the hash of the first value ...
    std::size_t hashedKey = std::hash<unsigned int>()(key.at(0));

    // ... and add the hash value of all following values to it.
    // Note: This is adapted from the Boost library (boost::hash_combine).
    for (const unsigned int& value : key) {
      hashedKey ^= std::hash<unsigned int>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
    }

    return hashedKey;
  }

  inline bool IsKeyEqual::operator() (
    const arma::Col<double>& firstKey,
    const arma::Col<double>& secondKey) const  {
    // Returns true if all values of both column vectors are equal.
    // This will also check if both vectors have the same size.
    return arma::all(firstKey == secondKey);
  }

  inline bool IsKeyEqual::operator() (
    const arma::Col<unsigned int>& firstKey,
    const arma::Col<unsigned int>& secondKey) const  {
    // Returns true if all values of both column vectors are equal.
    // This will also check if both vectors have the same size.
    return arma::all(firstKey == secondKey);
  }
}
