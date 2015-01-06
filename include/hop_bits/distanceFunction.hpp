#pragma once

// Armadillo
#include <armadillo>

namespace hop {
  template <typename ParameterType>
  class DistanceFunction {
    public:
      explicit DistanceFunction() = default;

      double getDistance(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter) const;

    protected:
      double getDistanceImplementation(
          const arma::Col<ParameterType>& firstParameter,
          const arma::Col<ParameterType>& secondParameter) const;
  };

  template <typename ParameterType>
  double DistanceFunction<ParameterType>::getDistance(
      const arma::Col<ParameterType>& firstParameter,
      const arma::Col<ParameterType>& secondParameter) const {
    if(firstParameter.n_elem != secondParameter.n_elem) {
      // TODO Add exception
    }

    return getDistanceImplementation(firstParameter, secondParameter);
  }
}
