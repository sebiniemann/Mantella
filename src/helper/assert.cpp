#include "mantella_bits/helper/assert.hpp"

// C++ standard library
#include <cmath>
#include <stdexcept>
#include <utility>

namespace mant {
  void verify(
      const bool expression,
      const std::string& errorMessage) {
    if (!expression) {
      throw std::logic_error(
          "Mantella: " + errorMessage);
    }
  }

  bool isRotationMatrix(
      const arma::Mat<double>& rotationMatrixCandidate) {
    // Is the rotation matrix square?
    if (!rotationMatrixCandidate.is_square()) {
      return false;
    }

    // Is its determinant either 1 or -1?
    if (std::abs(std::abs(arma::det(rotationMatrixCandidate)) - 1.0) > 1.0e-12) {
      return false;
    }

    // Is its transpose also its inverse?
    // For (nearly) singular matrices, the inversion might throw an exception.
    if (arma::any(arma::vectorise(arma::abs(arma::pinv(rotationMatrixCandidate).t() - rotationMatrixCandidate)) > 1.0e-12 * arma::max(arma::ones(rotationMatrixCandidate.n_elem), arma::vectorise(rotationMatrixCandidate)))) {
      return false;
    }

    return true;
  }

  bool isPermutation(
      const arma::Col<arma::uword>& permutationCandidate,
      const arma::uword numberOfPermutations,
      const arma::uword numberOfElements) {
    // Are there as more permutations than elements?
    if (numberOfPermutations > numberOfElements) {
      return false;
    }

    // Are there as many permutations as expected?
    if (permutationCandidate.n_elem != numberOfPermutations) {
      return false;
    }

    // Are all elements within [0, numberOfElements - 1]?
    if (arma::any(permutationCandidate < 0) || arma::any(permutationCandidate > numberOfElements - 1)) {
      return false;
    }

    // Are all elements unique?
    if (static_cast<arma::Col<arma::uword>>(arma::unique(permutationCandidate)).n_elem != permutationCandidate.n_elem) {
      return false;
    }

    return true;
  }

  bool isDimensionallyConsistent(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
    const arma::uword numberOfDimensions = samples.cbegin()->first.n_elem;
    for (const auto& sample : samples) {
      if (sample.first.n_elem != numberOfDimensions) {
        return false;
      }
    }

    return true;
  }
}
