#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <stdexcept>
#include <string>

// Mantella
#include "mantella_bits/combinatorics.hpp"
#include "mantella_bits/config.hpp"
#include "mantella_bits/geometry.hpp"
#include "mantella_bits/numericalAnalysis.hpp"
#include "mantella_bits/optimisationAlgorithm/hookeJeevesAlgorithm.hpp"
#include "mantella_bits/optimisationProblem.hpp"

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
        return (1.0 / factorial(static_cast<double>(type - 2)) - stumpffFunction(parameter, type - 2)) / parameter;
      }
    }

    // The following calculations are based on universal variables, following the work of Bate et al.
    // To understand the following equations, we advise to carefully read the following book:
    // @see R. Bate et al. (1971). Fundamentals of Astrodynamics. Dover Publications, ed. 1, pp. 191-197.
    double timeOfFlight(
        const double universalVariable,
        const arma::vec::fixed<3>& departurePosition,
        const arma::vec::fixed<3>& arrivalPosition,
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

      double timeOfFlight = (std::pow(y / secondStumpffValue, 1.5) * thirdStumpffValue + A * std::sqrt(y)) / std::sqrt(heliocentricGravitationalConstant);
      if (std::isnan(timeOfFlight)) {
        if (universalVariable > 0) {
          timeOfFlight = std::numeric_limits<double>::infinity();
        } else {
          timeOfFlight = -std::numeric_limits<double>::infinity();
        }
      }

      return timeOfFlight;
    }

    double sphereOfInfluenceRadius(
        const double semimajorAxis,
        const double mass) {
      return std::pow(mass / ::mant::itd::solarMass, 0.4) * semimajorAxis;
    }

    std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> positionAndVelocityOnOrbit(
        const double modifiedJulianDate,
        const arma::vec::fixed<7>& keplerianElements) {
      const double eccentricity = keplerianElements(1);
      if (eccentricity >= 1.0) {
        throw std::domain_error("positionAndVelocityOnOrbit: The eccentricity must be less than 1.");
      }

      const double semiMajorAxis = keplerianElements(0);
      const double inclination = keplerianElements(2);
      const double longitudeOfTheAscendingNode = keplerianElements(3);
      const double argumentOfPeriapsis = keplerianElements(4);
      const double meanAnomaly = keplerianElements(5) + (modifiedJulianDate - keplerianElements(6)) * std::sqrt(::mant::itd::heliocentricGravitationalConstant / std::pow(semiMajorAxis, 3.0));

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

    arma::vec::fixed<3> gravityAssist(
        const arma::vec::fixed<3>& satelliteVelocity,
        const arma::vec::fixed<3>& planetPosition,
        const arma::vec::fixed<3>& planetVelocity,
        const double standardGravitationalParameter,
        const double periapsis) {
      const double satelliteVelocityMagnitude = arma::norm(satelliteVelocity);
      const double excessVelocityMagnitude = arma::norm(satelliteVelocity - planetVelocity);
      const double alpha = std::acos(arma::norm_dot(satelliteVelocity, planetVelocity));
      const double phi_2 = std::atan2(satelliteVelocityMagnitude * std::cos(alpha) - arma::norm(planetVelocity), satelliteVelocityMagnitude * std::sin(alpha)) + 2 * std::asin(1.0 / (periapsis * std::pow(excessVelocityMagnitude, 2.0) / standardGravitationalParameter + 1.0));

      return excessVelocityMagnitude * std::cos(phi_2) * arma::normalise(planetVelocity) + excessVelocityMagnitude * std::sin(phi_2) * arma::normalise(planetPosition);
    }

    std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> lambert(
        const arma::vec::fixed<3>& departurePosition,
        const arma::vec::fixed<3>& arrivalPosition,
        const bool useProgradeTrajectory,
        const arma::uword numberOfRevolutions,
        const double transferTime) {
      if (transferTime < 0.0) {
        throw std::domain_error("lambert: The transfer time must be positive (including 0)");
      }

      if (transferTime < ::mant::machinePrecision) {
        if (arma::approx_equal(departurePosition, arrivalPosition, "absdiff", ::mant::machinePrecision)) {
          return {arma::ones<arma::vec>(3), arma::ones<arma::vec>(3)};
        } else {
          return {arma::ones<arma::vec>(3) + std::numeric_limits<double>::infinity(), arma::ones<arma::vec>(3) + std::numeric_limits<double>::infinity()};
        }
      }

      auto timeOfFlightFunction = [&departurePosition, &arrivalPosition, &useProgradeTrajectory, &transferTime](const double universalVariable) {
        return timeOfFlight(universalVariable, departurePosition, arrivalPosition, useProgradeTrajectory) - transferTime;
      };

      double universalVariable;
      if (numberOfRevolutions == 0) {
        const double previousMachinePrecision = ::mant::machinePrecision;
        ::mant::machinePrecision = std::max(::mant::machinePrecision, ::mant::machinePrecision * transferTime);
        universalVariable = brent(timeOfFlightFunction, -4.0 * arma::datum::pi, 4.0 * std::pow(arma::datum::pi, 2.0), 100);
        ::mant::machinePrecision = previousMachinePrecision;
      } else {
        OptimisationProblem optimisationProblem(1);
        optimisationProblem.setObjectiveFunctions({{[&timeOfFlightFunction](const arma::vec& universalVariable) {
                                                      return timeOfFlightFunction(universalVariable(0));
                                                    },
            "Time of flight difference"}});
        HookeJeevesAlgorithm optimisationAlgorithm;
        optimisationAlgorithm.setMaximalNumberOfIterations(100);
        optimisationAlgorithm.setAcceptableObjectiveValue(std::min(transferTime - 2.0 * ::mant::machinePrecision, std::nexttoward(transferTime, 0.0)));
        optimisationAlgorithm.optimise(optimisationProblem);

        if (!optimisationAlgorithm.isFinished()) {
          return {arma::ones<arma::vec>(3) + std::numeric_limits<double>::infinity(), arma::ones<arma::vec>(3) + std::numeric_limits<double>::infinity()};
        }

        const double previousMachinePrecision = ::mant::machinePrecision;
        ::mant::machinePrecision = std::max(::mant::machinePrecision, ::mant::machinePrecision * transferTime);
        if (useProgradeTrajectory) {
          universalVariable = brent(timeOfFlightFunction, 4.0 * std::pow(numberOfRevolutions * arma::datum::pi, 2.0), optimisationAlgorithm.getBestFoundParameter()(0), 100);
        } else {
          universalVariable = brent(timeOfFlightFunction, optimisationAlgorithm.getBestFoundParameter()(0), 4.0 * std::pow((numberOfRevolutions + 1) * arma::datum::pi, 2.0), 100);
        }
        ::mant::machinePrecision = previousMachinePrecision;
      }

      if (!std::isfinite(universalVariable)) {
        return {arma::ones<arma::vec>(3) + std::numeric_limits<double>::infinity(), arma::ones<arma::vec>(3) + std::numeric_limits<double>::infinity()};
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

      const double firstLagrangeCoefficient = 1.0 - y / departureDistanceToSun;
      const double secondLagrangeCoefficient = A * std::sqrt(y / ::mant::itd::heliocentricGravitationalConstant);
      const double firstLagrangeCoefficientDeriavte = std::sqrt(::mant::itd::heliocentricGravitationalConstant) / (departureDistanceToSun * arrivalDistanceToSun) * std::sqrt(y / secondStumpffValue) * (universalVariable * thirdStumpffValue - 1.0);
      const double secondLagrangeCoefficientDeriavte = 1.0 - y / arrivalDistanceToSun;

      arma::vec departureVelocity = 1.0 / secondLagrangeCoefficient * (arrivalPosition - firstLagrangeCoefficient * departurePosition);
      return {departureVelocity, firstLagrangeCoefficientDeriavte * departurePosition + secondLagrangeCoefficientDeriavte * departureVelocity};
    }
  }
}
