#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  class PNorm : public DistanceFunction {
   public:
    const arma::uword p_;
    
    explicit PNorm(
        const arma::uword numberOfDimensions,
        const arma::uword p);

    std::string toString() const override;

   protected:
    double getLengthImplementation(
        const arma::Col<double>& parameter) const override;
  };
}
