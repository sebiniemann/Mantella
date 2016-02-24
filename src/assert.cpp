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
      // Adds *Mantella - * to each error message, to better differentiate between messages from Mantella or other libraries.
      throw std::logic_error("Mantella - " + errorMessage);
    }
  }

  bool isRotationMatrix(
      const arma::Mat<double>& rotationCandidate) {
    if (!rotationCandidate.is_square()) {
      return false;
      // A rotation matrix must be square and ...
    } else if (arma::any(arma::vectorise(arma::abs(arma::pinv(rotationCandidate).t() - rotationCandidate)) > 1e-12 * std::max(1.0, arma::abs(rotationCandidate).max()))) {
      // ... its transpose must be equal to its inverse.
      return false;
    }

    return true;
  }

  bool isPermutationVector(
      const arma::Col<arma::uword>& permutationCandidate,
      const arma::uword numberOfElements,
      const arma::uword cycleSize) {
    if (cycleSize > numberOfElements) {
      // The number of element to be permuted cannot be larger than the number of elements, ...
      return false;
    } else if (permutationCandidate.n_elem != cycleSize) {
      // ... the number of element within a permutation matrix must be equal to the number of element to be permuted, ...
      return false;
    } else if (arma::any(permutationCandidate < 0) || arma::any(permutationCandidate > numberOfElements - 1)) {
      // ... all elements must be within [0, *numberOfElements* - 1] and ..
      return false;
    } else if (static_cast<arma::Col<arma::uword>>(arma::unique(permutationCandidate)).n_elem != permutationCandidate.n_elem) {
      // .. each element must be unique.
      return false;
    }

    return true;
  }

  bool isSymmetric(
      const arma::Mat<double>& symmetricCandidate) {
    if (!symmetricCandidate.is_square()) {
      return false;
    }

    for (arma::uword n = 0; n < symmetricCandidate.n_rows; ++n) {
      for (arma::uword k = n + 1; k < symmetricCandidate.n_cols; ++k) {
        if (std::abs(symmetricCandidate(n, k) - symmetricCandidate(k, n)) > 1e-12) {
          return false;
        }
      }
    }

    return true;
  }

  bool isPositiveSemiDefinite(
      const arma::Mat<double>& positiveSemiMatrixCandidate) {
    if (!positiveSemiMatrixCandidate.is_square()) {
      return false;
    }

    // A matrix is positive semi-definite, if all eigenvalues are at least 0.
    arma::Col<arma::cx_double> eigenValues;
    arma::eig_gen(eigenValues, positiveSemiMatrixCandidate);

    return arma::all(arma::real(eigenValues) >= 0) && arma::all(arma::imag(eigenValues) == 0);
  }

  bool isDimensionallyConsistent(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
    if (samples.size() < 1) {
      return true;
    }

    // We need to check each parameter in *samples* to be sure, that all have the same amount of elements.
    // The number of elements within the first parameter is used as reference.
    const arma::uword numberOfDimensions = samples.cbegin()->first.n_elem;
    for (const auto& sample : samples) {
      if (sample.first.n_elem != numberOfDimensions) {
        return false;
      }
    }

    return true;
  }
}
