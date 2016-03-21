#pragma once

// C++ standard library
#include <utility>
#include <vector>

// Armadillo
#include <armadillo>

namespace mant {
  namespace itd {

    const double refModifiedJulianDay2000 = 0.5;
    const double standardGravitationalParameterOfSun = 1.32712440018e20;

    std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> lambert(
        const arma::Col<double>::fixed<3>& departurePosition,
        const arma::Col<double>::fixed<3>& arrivalPosition,
        const double transferTime);
  }
}
