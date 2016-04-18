#pragma once

// C++ standard library
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  namespace itd {

    const double standardGravitationalParameterOfSun = 1.32712440018e20;

    std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> orbitOnPosition(
        const double modifiedJulianDay2000,
        const arma::Col<double>::fixed<6>& keplerianElements);
        
    std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>> orbitOnPosition(
        const double modifiedJulianDay2000,
        const arma::Mat<double>::fixed<2, 6>& keplerianElements);
  }
}
