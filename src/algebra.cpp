#include "mantella_bits/algebra.hpp"

// Mantella
#include "mantella_bits/combinatorics.hpp"

namespace mant {
  arma::uword polynomialSize(
      const arma::uword numberOfElements,
      const arma::uword polynomialOrder) {
    // The number of parameter combinations for the constant term.
    arma::uword polynomialSize = 1;
    // Sums up the number of parameter combinations for each degree > 0.
    for (arma::uword d = 1; d <= polynomialOrder; ++d) {
      polynomialSize += nchoosek(numberOfElements + d - 1, d);
    }

    return polynomialSize;
  }

  arma::Col<double> polynomial(
      const arma::Col<double>& parameter,
      const arma::uword polynomialOrder) {
    arma::Col<double> polynomial(polynomialSize(parameter.n_elem, polynomialOrder));

    // For any polynomial, all coefficients are 1.
    switch (polynomialOrder) {
      case 0: { // Constant polynomials
        // Constant term
        polynomial(0) = 1.0;
      } break;
      case 1: { // Linear polynomials
        // Linear term
        polynomial.head(parameter.n_elem) = parameter;
        // Constant term
        polynomial(parameter.n_elem) = 1;
      } break;
      case 2: { // Quadratic polynomials
        // Quadratic term
        arma::uword n = 0;
        for (arma::uword k = 0; k < parameter.n_elem; ++k) {
          for (arma::uword l = k; l < parameter.n_elem; ++l) {
            polynomial(n++) = parameter(k) * parameter(l);
          }
        }
        // Linear term
        polynomial.subvec(n, n + parameter.n_elem - 1) = parameter;
        // Constant term
        polynomial(polynomial.n_elem - 1) = 1;
      } break;
      case 3: { // Cubic polynomials
        // Cubic term
        arma::uword n = 0;
        for (arma::uword k = 0; k < parameter.n_elem; ++k) {
          for (arma::uword l = k; l < parameter.n_elem; ++l) {
            for (arma::uword m = l; m < parameter.n_elem; ++m) {
              polynomial(n++) = parameter(k) * parameter(l) * parameter(m);
            }
          }
        }
        // Quadratic term
        for (arma::uword k = 0; k < parameter.n_elem; ++k) {
          for (arma::uword l = k; l < parameter.n_elem; ++l) {
            polynomial(n++) = parameter(k) * parameter(l);
          }
        }
        // Linear term
        polynomial.subvec(n, n + parameter.n_elem - 1) = parameter;
        // Constant term
        polynomial(polynomial.n_elem - 1) = 1;
      } break;
      default: { // Polynomials of degree >= 4
        // All terms, expect the linear and constant one.
        arma::uword n = 0;
        // Generates the term for all degrees > 1
        for (arma::uword d = 2; d <= polynomialOrder; ++d) {
          for (const auto& multicombination : multicombinations(parameter.n_elem, d)) {
            polynomial(n++) = arma::prod(parameter.elem(multicombination));
          }
        }
        // Linear term
        polynomial.subvec(n, n + parameter.n_elem - 1) = parameter;
        // Constant term
        polynomial(polynomial.n_elem - 1) = 1;
      } break;
    }

    return polynomial;
  }
}
