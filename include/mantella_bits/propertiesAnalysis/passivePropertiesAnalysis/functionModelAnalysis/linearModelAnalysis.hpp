namespace mant {
  template <typename ParameterType>
  class LinearModelAnalysis : public FunctionModelAnalysis<ParameterType> {
    public:
      explicit LinearModelAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      LinearModelAnalysis(const LinearModelAnalysis&) = delete;
      LinearModelAnalysis& operator=(const LinearModelAnalysis&) = delete;

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
  LinearModelAnalysis<ParameterType>::LinearModelAnalysis() noexcept
    : isLinear_(false) {
    setLinearModelMedianErrorThreshold(0.25);
  }

  template <typename ParameterType>
  arma::Col<double> LinearModelAnalysis<ParameterType>::getLinearModelEstimator() const noexcept {
    return linearModelEstimator_;
  }

  template <typename ParameterType>
  void LinearModelAnalysis<ParameterType>::setLinearModelMedianErrorThreshold(
      const double& linearModelMedianErrorThreshold) {
    if(linearModelMedianErrorThreshold < 0 || linearModelMedianErrorThreshold > 1) {
      throw std::logic_error("The quadratic model MeanError threshold (" + std::to_string(linearModelMedianErrorThreshold) + ") must be within 0 and 1.");
    }

    linearModelMedianErrorThreshold_ = linearModelMedianErrorThreshold;
  }

  template <typename ParameterType>
  bool LinearModelAnalysis<ParameterType>::isLinear() const noexcept {
    return isLinear_;
  }
}
