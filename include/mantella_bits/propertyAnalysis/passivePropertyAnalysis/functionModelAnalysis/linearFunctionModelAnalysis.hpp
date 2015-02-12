namespace mant {
  template <typename ParameterType>
  class LinearFunctionModelAnalysis : public FunctionModelAnalysis<ParameterType> {
    public:
      explicit LinearFunctionModelAnalysis() noexcept;

      void setLinearModelMedianErrorThreshold(
          const double& linearModelMedianErrorThreshold);

      arma::Col<double> getLinearModelEstimator() const noexcept;

      bool isLinear() const noexcept;

    protected:
      arma::Col<double> linearModelEstimator_;
      bool isLinear_;

      double linearModelMedianErrorThreshold_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  LinearFunctionModelAnalysis<ParameterType>::LinearFunctionModelAnalysis() noexcept
    : isLinear_(false) {
    setLinearModelMedianErrorThreshold(0.25);
  }

  template <typename ParameterType>
  arma::Col<double> LinearFunctionModelAnalysis<ParameterType>::getLinearModelEstimator() const noexcept {
    return linearModelEstimator_;
  }

  template <typename ParameterType>
  void LinearFunctionModelAnalysis<ParameterType>::setLinearModelMedianErrorThreshold(
      const double& linearModelMedianErrorThreshold) {
    if(linearModelMedianErrorThreshold < 0 || linearModelMedianErrorThreshold > 1) {
      throw std::logic_error("The quadratic model MeanError threshold (" + std::to_string(linearModelMedianErrorThreshold) + ") must be within 0 and 1.");
    }

    linearModelMedianErrorThreshold_ = linearModelMedianErrorThreshold;
  }

  template <typename ParameterType>
  bool LinearFunctionModelAnalysis<ParameterType>::isLinear() const noexcept {
    return isLinear_;
  }
}
