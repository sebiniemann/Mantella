#pragma once

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  class EuclideanDistance : public DistanceFunction<double> {
    protected:
      double getDistanceImplementation(
          const arma::Col<double>& firstParameter,
          const arma::Col<double>& secondParameter) const override;
  };
}
