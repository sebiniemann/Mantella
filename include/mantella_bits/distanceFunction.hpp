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

      ParameterType getDistance(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter) const;

      arma::Col<ParameterType> getNeighour(
          const arma::Col<ParameterType>& parameter,
          const arma::Col<ParameterType>& minimalDistance,
          const arma::Col<ParameterType>& maximalDistance) const;

      arma::Col<ParameterType> getNeighour(
          const arma::Col<ParameterType>& parameter,
          const arma::Col<ParameterType>& maximalDistance) const;

    protected:
      virtual ParameterType getDistanceImplementation(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter) const noexcept = 0;

      virtual arma::Col<ParameterType> getNeighourImplementation(
          const arma::Col<ParameterType>& parameter,
          const arma::Col<ParameterType>& minimalDistance,
          const arma::Col<ParameterType>& maximalDistance) const = 0;
  };

  template <typename ParameterType>
  ParameterType DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& firstParameter,
      const arma::Col<ParameterType>& secondParameter) const {
    if(firstParameter.n_elem != secondParameter.n_elem) {
      throw std::logic_error("The number of dimensions of the first parameter (" + std::to_string(firstParameter.n_elem) + ") must match the number of dimensions of the second parameter (" + std::to_string(secondParameter.n_elem) + ").");
    }

    return getDistanceImplementation(firstParameter, secondParameter);
  }

  template <typename ParameterType>
  arma::Col<ParameterType> DistanceFunction<ParameterType>::getNeighour(
      const arma::Col<ParameterType>& parameter,
      const arma::Col<ParameterType>& minimalDistance,
      const arma::Col<ParameterType>& maximalDistance) const {
    if(minimalDistance.n_elem != maximalDistance.n_elem) {
      throw std::logic_error("The number of dimensions of the minimal distance (" + std::to_string(minimalDistance.n_elem) + ") must match the number of dimensions of the maximal distance (" + std::to_string(maximalDistance.n_elem) + ").");
    } else if(parameter.n_elem != minimalDistance.n_elem) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the minimal/maximal distance (" + std::to_string(minimalDistance.n_elem) + ").");
    } else if(arma::any(minimalDistance < 0)) {
      throw std::logic_error("Each minimal distance must be strict greater than 0.");
    } else if (arma::any(maximalDistance < minimalDistance)) {
      throw std::logic_error("Each maximal distance must greater or equal than its corresponding minimal distance.");
    }

    return getNeighourImplementation(minimalDistance, maximalDistance);
  }

  template <typename ParameterType>
  arma::Col<ParameterType> DistanceFunction<ParameterType>::getNeighour(
      const arma::Col<ParameterType>& parameter,
      const arma::Col<ParameterType>& maximalDistance) const {

    return getNeighour(parameter, arma::zeros<arma::Col<double>>(parameter.n_elem), maximalDistance);
  }
}
