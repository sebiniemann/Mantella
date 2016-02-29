#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <cmath>

// Mantella
#include "mantella_bits/numericalAnalysis.hpp"

namespace mant {
  namespace itd {
    std::pair<double, double> gravityAssist(
        const arma::Col<double>::fixed<3>& inboundVelocity,
        const arma::Col<double>::fixed<3>& outboundVelocity) {
      double inboundVelocityLength = arma::norm(inboundVelocity);
      double outboundVelocityLength = arma::norm(outboundVelocity);

      double velocitiesDotProduct = arma::dot(inboundVelocity, outboundVelocity);
      double alpha = std::acos(velocitiesDotProduct / (inboundVelocityLength * outboundVelocityLength));

      double inboundAcceleration = 1.0 / std::pow(inboundVelocityLength, 2.0);
      double outboundAcceleration = 1.0 / std::pow(outboundVelocityLength, 2.0);

      double rp = mant::brent(
          [&inboundAcceleration, &outboundAcceleration, &alpha](double parameter) { 
              return std::asin(inboundAcceleration / (inboundAcceleration + parameter)) + std::asin(outboundAcceleration / (outboundAcceleration + parameter)) - alpha;
          },
          0.0, 1.0, 100, 1e-10);

      double DV = std::abs(std::sqrt(std::pow(outboundVelocityLength, 2) + 2.0 / rp) - std::sqrt(std::pow(inboundVelocityLength, 2) + 2.0 / rp));

      return {DV, rp};
    }
  }
}
