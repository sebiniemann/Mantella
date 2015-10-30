#include <mantella_bits/distanceFunction/pNorm.hpp>

// C++ standard library
#include <string>

// Mantella
#include <mantella_bits/distanceFunction.hpp>

namespace mant {
  PNorm::PNorm(
      const arma::uword numberOfDimensions,
      const arma::uword p)
    : DistanceFunction(numberOfDimensions),
      p_(p) {
    
  }

  double PNorm::getLengthImplementation(
      const arma::Col<double>& parameter) const {
    return arma::norm(parameter, p_);
  }
  
  std::string PNorm::toString() const {
    return std::to_string(p_) + "_norm";
  }
}
