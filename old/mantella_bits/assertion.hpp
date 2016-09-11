#pragma once

namespace mant {
  template <typename T, std::size_t N>
  bool isRotationMatrix(
      const arma::array<T, N>& rotationCandidate);

  template <typename T1, typename T2, typename T3, std::size_t N>
  bool isPermutationVector(
      const arma::array<T1, N>& permutationCandidate,
      const T2 numberOfElements,
      const T3 cycleSize);
  template <typename T1, typename T2, std::size_t N>
  bool isPermutationVector(
      const arma::array<T1, N>& permutationCandidate,
      const T2 numberOfElements);

  //
  // Implementation
  //
      
  template <typename T, std::size_t N>
  bool isRotationMatrix(
      const arma::array<T, N>& rotationCandidate) {
    if (N < 4) {
      //  A rotation matrix must be have at least 2 dimensions, ...
      return false;
    } else if (!rotationCandidate.is_finite()) {
      //  ... be finite, ...
      return false;
    } else if (std::sqrt(N)) {
      // ... square, ...
      return false;
    } else {
      arma::mat inverse;
      // Uses the Moore-Penrose pseudo-inverse instead of the actual inverse, as the matrix might not be invertible.
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

  template <typename T1, typename T2, typename T3, std::size_t N>
  bool isPermutationVector(
      const arma::array<T1, N>& permutationCandidate,
      const T2 numberOfElements,
      const T3 cycleSize) {
    if (N == 0) {
      // A permutation over `cycleSize` elements from [0, `numberOfElements` - 1] must be non-empty, ...
      return false;
    } else if (N != cycleSize) {
      // ... have exactly `cycleSize` elements, ...
      return false;
    } else if (numberOfElements > 0 && (arma::any(permutationCandidate < 0) || arma::any(permutationCandidate > numberOfElements - 1))) {
      // ... have only elements from [0, `numberOfElements` - 1] and ..
      return false;
    } else if (arma::size(arma::find_unique(permutationCandidate)) != arma::size(permutationCandidate)) {
      // .. all elements must be unique.
      return false;
    }

    return true;
  }

  template <typename T1, typename T2, std::size_t N>
  bool isPermutationVector(
      const arma::array<T1, N>& permutationCandidate,
      const T2 numberOfElements) {
    return isPermutationVector(permutationCandidate, numberOfElements, numberOfElements);
  }
}
