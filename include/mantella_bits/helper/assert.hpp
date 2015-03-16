namespace mant {
  inline bool isRotationMatrix(
      const arma::Mat<double>& matrix) noexcept;

  inline bool isCompatibleDimension(
      const std::size_t firstDimension,
      const std::size_t secondDimension) noexcept;

  inline void checkRotationMatrix(
      const std::string& name,
      const arma::Mat<double>& matrix);

  inline void checkCompatibleDimension(
      const std::string& firstName,
      const std::size_t firstDimension,
      const std::string& secondName,
      const std::size_t secondDimension);

  inline bool isRotationMatrix(
      const arma::Mat<double>& matrix) noexcept {
    // is suqare?
    if (!matrix.is_square()) {
      return false;
    // is orthonormal?
    } else if(arma::any(arma::vectorise(arma::abs(matrix.i() - matrix.t()) > 1.0e-12 * std::max(1.0, std::abs(arma::median(arma::vectorise(matrix))))))) {
      return false;
    // determinant is either 1 or -1?
    } else if(std::abs(std::abs(arma::det(matrix)) - 1.0) > 1.0e-12) {
      return false;
    }

    return true;
  }

  inline bool isCompatibleDimension(
      const std::size_t firstDimension,
      const std::size_t secondDimension) noexcept {
    return (firstDimension == secondDimension);
  }

  inline void checkRotationMatrix(
      const std::string& name,
      const arma::Mat<double>& matrix) {
    if(!isRotationMatrix(matrix)) {
      throw std::logic_error("Rotation matrix required: " + name + " must be square, orthonormal and its determinant be either 1 or -1.");
    }
  }

  inline void checkCompatibleDimension(
      const std::string& firstName,
      const std::size_t firstDimension,
      const std::string& secondName,
      const std::size_t secondDimension) {
    if(!isCompatibleDimension(firstDimension, secondDimension)) {
      throw std::logic_error("Incompatible dimensions: " + firstName + " must be equal to " + secondName);
    }
  }
}
