namespace mant {
  inline arma::Mat<double> getRandomRotationMatrix(
      const unsigned int numberOfDimensions) noexcept;

  inline arma::Col<unsigned int> getRandomPermutation(
      const unsigned int numberOfElements,
      const unsigned int cycleLength);

  inline arma::Col<unsigned int> getRandomPermutation(
      const unsigned int numberOfElements) noexcept;

  //
  // Implementation
  //

  inline arma::Mat<double> getRandomRotationMatrix(
      const unsigned int numberOfDimensions) noexcept {
    arma::Mat<double> Q;
    arma::Mat<double> R;

    if(arma::qr(Q, R, arma::randn<arma::Mat<double>>(numberOfDimensions, numberOfDimensions))) {
      return Q * arma::sign(arma::diagmat(R));
    } else {
      const arma::Col<double>& angles = arma::datum::pi * arma::randu<arma::Col<double>>(numberOfDimensions);
      const arma::Col<double>& sineAngles = arma::sin(angles);
      const arma::Col<double>& cosineAngles = arma::cos(angles);

      arma::Mat<double> rotationMatrix = arma::eye<arma::Mat<double>>(numberOfDimensions, numberOfDimensions);
      for(std::size_t n = 0; n < angles.n_elem; ++n) {
        arma::Mat<double> subRotationMatrix = arma::eye<arma::Mat<double>>(numberOfDimensions, numberOfDimensions);
        subRotationMatrix(0, 0) = sineAngles(n);
        subRotationMatrix(0, n) = cosineAngles(n);
        subRotationMatrix(n, 0) = cosineAngles(n);
        subRotationMatrix(n, n) = -sineAngles(n);

        rotationMatrix *= subRotationMatrix;
      }

      return rotationMatrix;
    }
  }

  inline arma::Col<unsigned int> getRandomPermutation(
      const unsigned int numberOfElements,
      const unsigned int cycleSize) {
    verify(cycleSize <= numberOfElements , "The cycle size must be lower than or equal to the number of elements.");

    arma::Col<unsigned int> permutation(numberOfElements);
    for (std::size_t n = 0; n < numberOfElements; ++n) {
      permutation(n) = n;
    }

    unsigned int length = std::min(cycleSize, numberOfElements - 1);
    for (std::size_t n = 0; n < length; ++n) {
      permutation.swap_rows(n, std::uniform_int_distribution<unsigned int>(n, length)(Rng::getGenerator()));
    }

    return permutation.head(cycleSize);
  }

  inline arma::Col<unsigned int> getRandomPermutation(
      const unsigned int numberOfElements) noexcept {
    return getRandomPermutation(numberOfElements, numberOfElements);
  }
}
