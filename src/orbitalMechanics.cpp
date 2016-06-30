#include "mantella_bits/orbitalMechanics.hpp"

// C++ standard library
#include <cmath>

// Mantella
#include "mantella_bits/combinatorics.hpp"

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
  }
}
