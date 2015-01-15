#pragma once

// C++ Standard Library
#include <stdexcept>

// Armadillo
#include <armadillo>

namespace mant {
  template <typename ParameterType>
  class DistanceFunction {
    public:
      explicit DistanceFunction() = default;

      double getDistance(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter) const;

      arma::Col<ParameterType> getNeighour(
          const double& minimalDistance,
          const double& maximalDistance) const noexcept;

    protected:
      virtual double getDistanceImplementation(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter) const = 0;

      virtual arma::Col<ParameterType> getNeighourImplementation(
          const double& minimalDistance,
          const double& maximalDistance) const noexcept = 0;
  };

  template <typename ParameterType>
  double DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& firstParameter,
      const arma::Col<ParameterType>& secondParameter) const {
    if(firstParameter.n_elem != secondParameter.n_elem) {
      throw std::logic_error("The number of dimensions of the fist parameter (" + std::to_string(firstParameter.n_elem) + ") must match the number of dimensions of the second parameter (" + std::to_string(secondParameter.n_elem) + ").");
    }

    return getDistanceImplementation(firstParameter, secondParameter);
  }

  template <typename ParameterType>
  arma::Col<ParameterType> DistanceFunction<ParameterType>::getNeighour(
      const double& minimalDistance,
      const double& maximalDistance) const {
    if(minimalDistance < 0) {
      throw std::logic_error("The minimal distance (" + std::to_string(minimalDistance) + ") must be strict greater than 0.");
    } else if (maximalDistance < minimalDistance) {
      throw std::logic_error("The maximal distance (" + std::to_string(maximalDistance) + ") must greater or equal than the minimal distance (" + std::to_string(minimalDistance) + ").");
    }

    return getNeighourImplementation(minimalDistance, maximalDistance);
  }
}
