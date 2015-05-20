namespace mant {
  inline void verify(
      const bool expression,
      const std::string& errorMessage);

  template <typename T>
  bool isRotationMatrix(
      const arma::Mat<T>& parameter) noexcept;

  inline bool isPermutation(
      const arma::Col<unsigned int>& parameter,
      const unsigned int lowerBound,
      const unsigned int upperBound) noexcept;

  //
  // Implementation
  //
      
  inline void verify(
      const bool expression,
      const std::string& errorMessage) {
    if(!expression) {
      throw std::runtime_error(errorMessage);
    }   
  }

  template <typename T>
  bool isRotationMatrix(
      const arma::Mat<T>& parameter) noexcept {
    static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");

    // is suqare?
    if (!parameter.is_square()) {
      return false;
    // is orthonormal?
    } else if(arma::any(arma::vectorise(arma::abs(parameter.i() - parameter.t()) > 1.0e-12 * std::max(1.0, std::abs(arma::median(arma::vectorise(parameter))))))) {
      return false;
    // determinant is either 1 or -1?
    } else if(std::abs(std::abs(arma::det(parameter)) - 1.0) > 1.0e-12) {
      return false;
    }

    return true;
  }

  inline bool isPermutation(
      const arma::Col<unsigned int>& parameter,
      const unsigned int lowerBound,
      const unsigned int upperBound) noexcept {
    // TODO Add logic
    return true;
  }
}
