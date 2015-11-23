#include "mantella_bits/assert.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace mant {
  void verify(
      const bool expression,
      const std::string& errorMessage) {
    if (!expression) {
      // Adds *Mantella - * to each error message, to better different between messages from Mantella or other libraries.
      throw std::logic_error("Mantella - " + errorMessage);
    }
  }

  bool isRotationMatrix(
      const arma::Mat<double>& rotationCandidate) {
// Expensive asserts are only to be run in debug mode.
#if !defined(NDEBUG)
    // Rotation matrices must be square and ...
    if (!rotationCandidate.is_square()) {
      return false;
      // ... orthogonal ...
    } else if (std::abs(std::abs(arma::det(rotationCandidate)) - 1.0) > 1.0e-12) {
      return false;
      // ... with determinant 1 or -1 (including improper rotations).
    } else if (arma::any(arma::vectorise(arma::abs(arma::pinv(rotationCandidate).t() - rotationCandidate)) > 1.0e-12 * std::max(1.0, arma::abs(rotationCandidate).max()))) {
      return false;
    }
#endif

    return true;
  }

  bool isPermutationMatrix(
      const arma::Col<arma::uword>& permutationCandidate,
      const arma::uword numberOfPermutations,
      const arma::uword numberOfElements) {
// Expensive asserts are only to be run in debug mode.
#if !defined(NDEBUG)
    // The number of element to be permuted cannot be larger than the number of elements, ...
    if (numberOfPermutations > numberOfElements) {
      return false;
      // ... the number of element within a permutation matrix must be equal to the number of element to be permuted, ...
    } else if (permutationCandidate.n_elem != numberOfPermutations) {
      return false;
      // ... all elements must be within [0, *numberOfElements* - 1] and ...
    } else if (arma::any(permutationCandidate < 0) || arma::any(permutationCandidate > numberOfElements - 1)) {
      return false;
      // .. each element must be unique.
    } else if (static_cast<arma::Col<arma::uword>>(arma::unique(permutationCandidate)).n_elem != permutationCandidate.n_elem) {
      return false;
    }
#endif

    return true;
  }

  bool isSymmetric(
      const arma::Mat<double>& symmetricCandidate) {
// Expensive asserts are only to be run in debug mode.
#if !defined(NDEBUG)
    if (!symmetricCandidate.is_square()) {
      return false;
    }

    for (arma::uword n = 0; n < symmetricCandidate.n_rows; ++n) {
      for (arma::uword k = n + 1; k < symmetricCandidate.n_cols; ++k) {
        // Symmetric matrices should have perfectly equal pairs of values.
        if (std::abs(symmetricCandidate(n, k) - symmetricCandidate(k, n)) > 0) {
          return false;
        }
      }
    }
#endif

    return true;
  }

  bool isPositiveSemiDefinite(
      const arma::Mat<double>& positiveSemiMatrixCandidate) {
// Expensive asserts are only to be run in debug mode.
#if !defined(NDEBUG)
    if (!positiveSemiMatrixCandidate.is_square()) {
      return false;
    }

    // A matrix is positive semi-definite, if all eigenvalues are at least 0.
    // Calculating the eigenvalues is actually overkill for this task, but this function is anyway not intended to be used in productive codes (only for debug).
    arma::Col<arma::cx_double> eigenValues;
    arma::eig_gen(eigenValues, positiveSemiMatrixCandidate);

    return arma::all(arma::real(eigenValues) >= 0) && arma::all(arma::imag(eigenValues) == 0);
#else
    return true;
#endif
  }

  bool isDimensionallyConsistent(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
// Expensive asserts are only to be run in debug mode.
#if !defined(NDEBUG)
    // We need to check each parameter in *samples* to be sure, that all have the same amount of elements.
    // The number of elements within the first parameter is used as reference.
    const arma::uword numberOfDimensions = samples.cbegin()->first.n_elem;
    for (const auto& sample : samples) {
      if (sample.first.n_elem != numberOfDimensions) {
        return false;
      }
    }
#endif

    return true;
  }
}
