#pragma once

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

namespace mant {
  // Calculates a hash value for custom types.
  template <typename ParameterType>
  class Hash {
    public:
      explicit Hash() = default;

      Hash(const Hash&) = default;

      Hash& operator=(const Hash&) = delete;

      std::size_t operator() (
          const ParameterType& key) const noexcept;
    };

  // Checks whether two keys of a custom type are equal.
  template <typename ParameterType>
  class IsKeyEqual {
    public:
      explicit IsKeyEqual() = default;

      IsKeyEqual(const IsKeyEqual&) = default;

      IsKeyEqual& operator=(const IsKeyEqual&) = delete;

      bool operator() (
          const ParameterType& firstKey,
          const ParameterType& secondKey) const noexcept;
  };

  template <>
  std::size_t Hash<arma::Col<double>>::operator() (
    const arma::Col<double>& key) const noexcept;
  template <>
  std::size_t Hash<arma::Col<unsigned int>>::operator() (
    const arma::Col<unsigned int>& key) const noexcept;

  template <>
  bool IsKeyEqual<arma::Col<double>>::operator() (
    const arma::Col<double>& firstKey,
    const arma::Col<double>& secondKey) const noexcept;
  template <>
  bool IsKeyEqual<arma::Col<unsigned int>>::operator() (
    const arma::Col<unsigned int>& firstKey,
    const arma::Col<unsigned int>& secondKey) const noexcept;
}
