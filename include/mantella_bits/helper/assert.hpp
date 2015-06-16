namespace mant {
  inline void verify(
      const bool expression,
      const std::string& errorMessage);

  template <typename T>
  bool isRotationMatrix(
      const arma::Mat<T>& parameter) noexcept;

  inline bool isPermutation(
      const arma::Col<unsigned int>& parameter,
      const std::size_t numberOfPermutations,
      const std::size_t numberOfElements) noexcept;

  //
  // Implementation
  //
      
  inline void verify(
      const bool expression,
      const std::string& errorMessage) {
    if(!expression) {
      throw std::logic_error(errorMessage);
    }   
  }

  template <typename T>
  bool isRotationMatrix(
      const arma::Mat<T>& parameter) noexcept {
    // Is the rotation matrix square?
    if (!parameter.is_square()) {
      return false;
    }
      
    // Is its determinant either 1 or -1?
    if(std::abs(std::abs(arma::det(parameter)) - 1.0) > 1.0e-12) {
      return false;
    }
      
    // Is the rotation matrix square?
    // For (nearly) singular matrices, the inversion might throw an exception.
    try {
      if(arma::any(arma::vectorise(arma::abs(parameter.i() - parameter.t()) > 1.0e-12 * std::max(1.0, std::abs(arma::median(arma::vectorise(parameter))))))) {
        return false;
      }
    } catch (...) {
      return false;
    }

    return true;
  }

  inline bool isPermutation(
      const arma::Col<unsigned int>& parameter,
      const std::size_t numberOfPermutations,
      const std::size_t numberOfElements) noexcept {
    // Are there as many permutations as expected?
    if (parameter.n_elem != numberOfPermutations) {
      return false;
    }
    
    // Are all elements within [0, numberOfElements - 1]?
    if (arma::any(parameter < 0) || arma::any(parameter > numberOfElements - 1)) {
      return false;
    }
    
    // Are all elements unique?
    if (static_cast<arma::Col<unsigned int>>(arma::unique(parameter)).n_elem != parameter.n_elem) {
      return false;
    }
    
    return true;
  }
}
