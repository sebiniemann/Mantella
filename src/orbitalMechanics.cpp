#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <cmath>
#include <stdexcept>

// Mantella
#include "mantella_bits/combinatorics.hpp"
#include "mantella_bits/config.hpp"
#include "mantella_bits/geometry.hpp"
#include "mantella_bits/numericalAnalysis.hpp"

namespace mant {
  namespace itd {
    double stumpffFunction(
        const double parameter,
        const arma::uword type) {
      if (type == 0) {
        if (parameter > 0.0) {
          return std::cos(std::sqrt(parameter));
        } else if (parameter < 0.0) {
          return std::cosh(std::sqrt(-parameter));
        } else {
          return 0.5;
        }
      } else if (type == 1) {
        if (parameter > 0.0) {
          return std::sin(std::sqrt(parameter)) / std::sqrt(parameter);
        } else if (parameter < 0.0) {
          return std::sinh(std::sqrt(-parameter)) / std::sqrt(-parameter);
        } else {
          return 5.0 / 6.0;
        }
      } else {
        return (1.0 / factorial(type) - stumpffFunction(parameter, type - 2)) / parameter;
      }
    }

    // The following calculations are based on universal variables, following the work of Bate et al.
    // To understand the following equations, we advise to carefully read the following book:
    // @see R. Bate et al. (1971). Fundamentals of Astrodynamics. Dover Publications, ed. 1, pp. 191-197.
    double timeOfFlight(
        const double universalVariable,
        const arma::vec::fixed<3> departurePosition,
        const arma::vec::fixed<3> arrivalPosition,
        const bool useProgradeTrajectory) {
      if (arma::approx_equal(departurePosition, arrivalPosition, "absdiff", ::mant::machinePrecision)) {
        return 0.0;
      }

      const double departureDistanceToSun = arma::norm(departurePosition);
      const double arrivalDistanceToSun = arma::norm(arrivalPosition);

      double trueAnomaly = std::acos(arma::norm_dot(departurePosition, arrivalPosition));

      if (useProgradeTrajectory != arma::vec(arma::cross(departurePosition, arrivalPosition))(2) > 0) {
        trueAnomaly = 2.0 * arma::datum::pi - trueAnomaly;
      }

      const double secondStumpffValue = stumpffFunction(universalVariable, 2);
      const double thirdStumpffValue = stumpffFunction(universalVariable, 3);

      const double A = std::sin(trueAnomaly) * std::sqrt(departureDistanceToSun * arrivalDistanceToSun / (1.0 - std::cos(trueAnomaly)));
      const double y = departureDistanceToSun + arrivalDistanceToSun + (A * (universalVariable * thirdStumpffValue - 1.0)) / std::sqrt(secondStumpffValue);

      return (std::pow(y / secondStumpffValue, 1.5) * thirdStumpffValue + A * std::sqrt(y)) / std::sqrt(heliocentricGravitationalConstant);
    }

    std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> positionAndVelocityOnOrbit(
        const double modifiedJulianDay,
        const arma::vec::fixed<7>& keplerianElements) {
      const double eccentricity = keplerianElements(1);
      if (eccentricity >= 1.0) {
        throw std::domain_error("positionAndVelocityOnOrbit: The eccentricity must be less than 1.");
      }

      const double semiMajorAxis = keplerianElements(0);
      const double inclination = keplerianElements(2);
      const double longitudeOfTheAscendingNode = keplerianElements(3);
      const double argumentOfPeriapsis = keplerianElements(4);
      const double meanAnomaly = keplerianElements(5) + (modifiedJulianDay - keplerianElements(6)) * std::sqrt(::mant::itd::heliocentricGravitationalConstant / std::pow(semiMajorAxis, 3.0));

      const double eccentricAnomaly = mant::brent([&meanAnomaly, &eccentricity](
                                                      double parameter) {
        return parameter - eccentricity * std::sin(parameter) - meanAnomaly;
      },
          meanAnomaly + eccentricity * std::cos(meanAnomaly) - 1.0, meanAnomaly + eccentricity * std::cos(meanAnomaly) + 1.0, 100);

      if (std::isnan(eccentricAnomaly)) {
        throw std::runtime_error("positionAndVelocityOnOrbit: Could not solve for the eccentric anomaly.");
      }

      const double y = semiMajorAxis * std::sqrt(1.0 - std::pow(eccentricity, 2.0));
      const double n = std::sqrt(::mant::itd::heliocentricGravitationalConstant / std::pow(semiMajorAxis, 3.0));

      const arma::mat& rotationMatrix = rotationMatrix3dExtrinsic(longitudeOfTheAscendingNode, inclination, argumentOfPeriapsis);

      return {rotationMatrix * arma::vec({semiMajorAxis * (std::cos(eccentricAnomaly) - eccentricity), y * std::sin(eccentricAnomaly), 0.0}), rotationMatrix * arma::vec({-(semiMajorAxis * n * std::sin(eccentricAnomaly)) / (1.0 - eccentricity * std::cos(eccentricAnomaly)), (y * n * std::cos(eccentricAnomaly)) / (1.0 - eccentricity * std::cos(eccentricAnomaly)), 0.0})};
    }
  }
}
