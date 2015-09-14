#pragma once

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  class MahalabonbisDistance : public DistanceFunction {
    public:
      using DistanceFunction::DistanceFunction;
    
      std::string toString() const;
    
    protected:
      double getLengthImplementation(
          const arma::Col<double>& parameter) const override;
  };
}