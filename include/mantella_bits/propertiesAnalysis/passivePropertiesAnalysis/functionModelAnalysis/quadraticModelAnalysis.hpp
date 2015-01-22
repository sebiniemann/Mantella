namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class QuadraticModelAnalysis : public FunctionModelAnalysis<ParameterType, DistanceFunction> {
    public:
      explicit QuadraticModelAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      QuadraticModelAnalysis(const QuadraticModelAnalysis&) = delete;
      QuadraticModelAnalysis& operator=(const QuadraticModelAnalysis&) = delete;

      void setQuadraticModelMedianErrorThreshold(
          const double& quadraticModelMedianErrorThreshold);

      arma::Col<double> getQuadraticModelEstimator() const noexcept;

      bool isQuadratic() const noexcept;

    protected:
      arma::Col<double> quadraticModelEstimator_;
      bool isQuadratic_;

      double quadraticModelMedianErrorThreshold_;
  };

  template <typename ParameterType, class DistanceFunction>
  QuadraticModelAnalysis<ParameterType, DistanceFunction>::QuadraticModelAnalysis() noexcept
    : isQuadratic_(false) {
    setQuadraticModelMedianErrorThreshold(0.25);
  }

  template <typename ParameterType, class DistanceFunction>
  arma::Col<double> QuadraticModelAnalysis<ParameterType, DistanceFunction>::getQuadraticModelEstimator() const noexcept {
    return quadraticModelEstimator_;
  }

  // TODO Be more strict with runtime/logic_erros
  template <typename ParameterType, class DistanceFunction>
  void QuadraticModelAnalysis<ParameterType, DistanceFunction>::setQuadraticModelMedianErrorThreshold(
      const double& quadraticModelMedianErrorThreshold) {
    if(quadraticModelMedianErrorThreshold < 0 || quadraticModelMedianErrorThreshold > 1) {
      throw std::runtime_error("The quadratic model MeanError threshold (" + std::to_string(quadraticModelMedianErrorThreshold) + ") must be within 0 and 1.");
    }

    quadraticModelMedianErrorThreshold_ = quadraticModelMedianErrorThreshold;
  }

  template <typename ParameterType, class DistanceFunction>
  bool QuadraticModelAnalysis<ParameterType, DistanceFunction>::isQuadratic() const noexcept {
    return isQuadratic_;
  }
}
