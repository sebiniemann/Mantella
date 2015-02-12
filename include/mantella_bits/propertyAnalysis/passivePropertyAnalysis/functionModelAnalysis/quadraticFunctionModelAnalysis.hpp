namespace mant {
  template <typename ParameterType>
  class QuadraticFunctionModelAnalysis : public FunctionModelAnalysis<ParameterType> {
    public:
      explicit QuadraticFunctionModelAnalysis() noexcept;

      void setQuadraticModelMedianErrorThreshold(
          const double& quadraticModelMedianErrorThreshold);

      arma::Col<double> getQuadraticModelEstimator() const noexcept;

      bool isQuadratic() const noexcept;

    protected:
      arma::Col<double> quadraticModelEstimator_;
      bool isQuadratic_;

      double quadraticModelMedianErrorThreshold_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  QuadraticFunctionModelAnalysis<ParameterType>::QuadraticFunctionModelAnalysis() noexcept
    : isQuadratic_(false) {
    setQuadraticModelMedianErrorThreshold(0.25);
  }

  template <typename ParameterType>
  arma::Col<double> QuadraticFunctionModelAnalysis<ParameterType>::getQuadraticModelEstimator() const noexcept {
    return quadraticModelEstimator_;
  }

  // TODO Be more strict with runtime/logic_erros
  template <typename ParameterType>
  void QuadraticFunctionModelAnalysis<ParameterType>::setQuadraticModelMedianErrorThreshold(
      const double& quadraticModelMedianErrorThreshold) {
    if(quadraticModelMedianErrorThreshold < 0 || quadraticModelMedianErrorThreshold > 1) {
      throw std::logic_error("The quadratic model MeanError threshold (" + std::to_string(quadraticModelMedianErrorThreshold) + ") must be within 0 and 1.");
    }

    quadraticModelMedianErrorThreshold_ = quadraticModelMedianErrorThreshold;
  }

  template <typename ParameterType>
  bool QuadraticFunctionModelAnalysis<ParameterType>::isQuadratic() const noexcept {
    return isQuadratic_;
  }
}
