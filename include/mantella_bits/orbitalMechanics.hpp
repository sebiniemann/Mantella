#pragma once

// C++ standard library
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  namespace itd {
    const double heliocentricGravitationalConstant = 1.32712440018e20;

    double stumpffFunction(
        const double parameter,
        const arma::uword type);

    double timeOfFlight(
        const double universalVariable,
        const arma::vec::fixed<3> departurePosition,
        const arma::vec::fixed<3> arrivalPosition,
        const bool useProgradeTrajectory);

    std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> positionAndVelocityOnOrbit(
        const double modifiedJulianDate,
        const arma::vec::fixed<7>& keplerianElements);

    std::pair<arma::vec::fixed<3>, arma::vec::fixed<3>> lambert(
        const arma::vec::fixed<3>& departurePosition,
        const arma::vec::fixed<3>& arrivalPosition,
        const bool useProgradeTrajectory,
        const arma::uword numberOfRevolutions,
        const double transferTime);
  }
}
