#include "mantella_bits/assert.hpp"

// C++ standard library
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>

// Mantella
#include "mantella_bits/config.hpp"

namespace mant {
  bool isRepresentableAsInteger(
      double value) {
    return (value >= 0 && value <= std::numeric_limits<arma::uword>::max() && std::abs(std::trunc(value) - value) <= 0.0);
  }

  bool isRepresentableAsFloatingPoint(
      arma::uword value) {
    return (value <= std::numeric_limits<double>::max() && static_cast<decltype(value)>(static_cast<double>(value)) == value);
  }

  bool isRotationMatrix(
      const arma::mat& rotationCandidate) {
    if (rotationCandidate.n_elem < 4) {
      //  A rotation matrix must be have at least 2 dimensions, ...
      return false;
    } else if (!rotationCandidate.is_finite()) {
      //  ... be finite, ...
      return false;
    } else if (!rotationCandidate.is_square()) {
      // ... square, ...
      return false;
    } else {
      arma::mat inverse;
      // Uses the Moore-Penrose pseudo-inverse instead of `arma::inv(...)`, as the matrix might not be invertible.
      if (!arma::pinv(inverse, rotationCandidate)) {
        // ... invertible and ...
        return false;
      } else if (!arma::approx_equal(inverse.t(), rotationCandidate, "absdiff", ::mant::machinePrecision)) {
        // ... its transpose must be equal to its inverse.
        return false;
      }
    }

    return true;
  }

  bool isPermutationVector(
      const arma::uvec& permutationCandidate,
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    if (permutationCandidate.is_empty()) {
      // A permutation over `cycleSize` elements from [0, `numberOfElements` - 1] must be non-empty, ...
      return false;
    } else if (permutationCandidate.n_elem != cycleSize) {
      // ... have exactly `cycleSize` elements, ...
      return false;
    } else if (numberOfElements < 1 || arma::any(permutationCandidate < 0) || arma::any(permutationCandidate > numberOfElements - 1)) {
      // ... have only elements from [0, `numberOfElements` - 1] and ..
      return false;
    } else if (arma::size(arma::find_unique(permutationCandidate)) != arma::size(permutationCandidate)) {
      // .. all elements must be unique.
      return false;
    }

    return true;
  }

  bool isPermutationVector(
      const arma::uvec& permutationCandidate,
      const arma::uword numberOfElements) {
    return isPermutationVector(permutationCandidate, numberOfElements, numberOfElements);
  }

  bool isSymmetric(
      const arma::mat& symmetricCandidate) {
    if (symmetricCandidate.is_empty()) {
      //  A symmetric matrix must be non-empty, ...
      return false;
    } else if (!symmetricCandidate.is_square()) {
      // ... square, ...
      return false;
    } else if (symmetricCandidate.has_nan()) {
      // ... free of NaNs (as no NaN is equal to another) ...
      return false;
    }

    // ... be equal to its transpose.
    for (arma::uword n = 0; n < symmetricCandidate.n_rows; ++n) {
      for (arma::uword k = n + 1; k < symmetricCandidate.n_cols; ++k) {
        // As both elements are finite at this point, the result can not be NaN after a single subtraction.
        if (std::abs(symmetricCandidate(k, n) - symmetricCandidate(n, k)) >= ::mant::machinePrecision) {
          return false;
        }
      }
    }

    return true;
  }

  bool isPositiveSemiDefinite(
      const arma::mat& positiveSemiMatrixCandidate) {
    if (positiveSemiMatrixCandidate.is_empty()) {
      //  A positive semi-definite matrix must be non-empty, ...
      return false;
    } else if (!positiveSemiMatrixCandidate.is_square()) {
      // ... square and ...
      return false;
    }

    // ... all eigenvalues must be positive (including 0).
    arma::cx_vec eigenValues;
    if (!arma::eig_gen(eigenValues, positiveSemiMatrixCandidate)) {
      return false;
    }

    return arma::all(arma::real(eigenValues) >= 0) && arma::all(arma::imag(eigenValues) == 0);
  }

  bool isDimensionallyConsistent(
      const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples) {
    if (samples.empty()) {
      // By definition, empty sets of samples are dimensionally consistent.
      return true;
    }

    // Checks that each parameter in `samples` has the same amount of dimensions.
    // The parameter's first number of dimensions is used as reference.
    const arma::uword numberOfDimensions = samples.cbegin()->first.n_elem;
    for (const auto& sample : samples) {
      if (sample.first.n_elem != numberOfDimensions) {
        return false;
      }
    }

    return true;
  }
}
