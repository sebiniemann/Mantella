namespace mant {
  inline bool isRotationMatrix(
      const arma::Mat<double>& matrix) noexcept;

  inline void isRotationMatrix(
      const std::string& name,
      const arma::Mat<double>& matrix);

  inline bool isEqual(
      const std::size_t firstValue,
      const std::size_t secondValue) noexcept;

  inline void isEqual(
      const std::string& firstName,
      const std::size_t firstValue,
      const std::string& secondName,
      const std::size_t secondValue);

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

  inline void isRotationMatrix(
      const std::string& name,
      const arma::Mat<double>& matrix) {
    if(!isRotationMatrix(matrix)) {
      throw std::logic_error(name + " must be a rotation matrix, i.e. it must be square, orthonormal and its determinant be either 1 or -1.");
    }
  }

  inline bool isEqual(
      const std::size_t firstValue,
      const std::size_t secondValue) noexcept {
    return (firstValue == secondValue);
  }

  inline void isEqual(
      const std::string& firstName,
      const std::size_t firstValue,
      const std::string& secondName,
      const std::size_t secondValue) {
    if(!isEqual(firstValue, secondValue)) {
      throw std::logic_error(firstName + " must be equal to " + secondName);
    }
  }
}
