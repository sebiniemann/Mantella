#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  class MahalanobisDistance : public DistanceFunction {
   public:
    const arma::Mat<double> covariance_;
   
    explicit MahalanobisDistance(
        const arma::Mat<double>& covariance);

    std::string toString() const override;

   protected:
    double getLengthImplementation(
        const arma::Col<double>& parameter) const override;
  };
}
