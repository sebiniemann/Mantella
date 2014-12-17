#pragma once

// HOP
#include <hop_bits/distanceFunction.hpp>

namespace hop {
  class EuclideanDistance : public DistanceFunction<double> {
    protected:
      double getDistanceImplementation(
          const arma::Col<double>& firstParameter,
          const arma::Col<double>& secondParameter) const;
  };
}
