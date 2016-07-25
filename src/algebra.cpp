#include "mantella_bits/algebra.hpp"

// C++ standard library
#include <limits>
#include <stdexcept>

// Mantella
#include "mantella_bits/combinatorics.hpp"

namespace mant {
  arma::uword polynomialSize(
      const arma::uword numberOfIndeterminates,
      const arma::uword highestDegree) {
    if (numberOfIndeterminates == 0 || highestDegree == 0) {
      return 1;
    }

    // Gets initialised with the number of constant terms (degree = 0).
    arma::uword polynomialSize = 1;
    // Sums up the number of parameter combinations for each degree > 0.
    for (arma::uword degree = 1; degree <= highestDegree; ++degree) {
      arma::uword numberOfCombinations;
      try {
        numberOfCombinations = nchoosek(numberOfIndeterminates + degree - 1, degree);
      } catch (const std::overflow_error& exception) {
        throw std::overflow_error("polynomialSize: The polynomial size will be greater than the largest supported integer.");
      }

      if (std::numeric_limits<decltype(polynomialSize)>::max() - polynomialSize < numberOfCombinations) {
        throw std::overflow_error("polynomialSize: The polynomial size will be greater than the largest supported integer.");
      }

      polynomialSize += numberOfCombinations;
    }

    return polynomialSize;
  }

  arma::vec polynomial(
      const arma::vec& parameter,
      const arma::uword highestDegree) {
    if (parameter.is_empty() || highestDegree == 0) {
      // By definition, the constant term is 1.
      return {1.0};
    }

    arma::vec polynomial(polynomialSize(parameter.n_elem, highestDegree));

    arma::uword n = 0;
    // Generates all terms for degree > 1
    for (arma::uword degree = highestDegree; degree >= 2; --degree) {
      for (const auto& multicombination : multicombinations(parameter.n_elem, degree)) {
        const arma::vec& partialParameter = parameter.elem(multicombination);

        polynomial(n++) = arma::prod(partialParameter);
      }
    }
    // Linear term
    polynomial.subvec(n, n + parameter.n_elem - 1) = parameter;
    // By definition, the constant term is 1.
    polynomial(polynomial.n_elem - 1) = 1;

    return polynomial;
  }
}
