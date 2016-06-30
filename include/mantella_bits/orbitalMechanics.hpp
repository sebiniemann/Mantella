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

    std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> positionOnOrbit(
        const double modifiedJulianDay2000,
        const arma::Col<double>::fixed<7>& keplerianElements);
  }
}
