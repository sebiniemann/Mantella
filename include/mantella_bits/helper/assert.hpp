namespace mant {
  inline void verify(
      const bool test,
      const std::string& errorMessage);

  inline bool isRotationMatrix(
      const arma::Mat<double>& matrix) noexcept;

  //
  // Implementation
  //
      
  inline void verify(
      const bool test,
      const std::string& errorMessage) {
    if(!test) {
      throw std::runtime_exception(errorMessage);
    }   
  }
      
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
}
