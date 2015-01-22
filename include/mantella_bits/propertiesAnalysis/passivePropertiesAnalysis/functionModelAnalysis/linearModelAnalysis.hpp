namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class LinearModelAnalysis : public FunctionModelAnalysis<ParameterType, DistanceFunction> {
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

  template <typename ParameterType, class DistanceFunction>
  LinearModelAnalysis<ParameterType, DistanceFunction>::LinearModelAnalysis() noexcept
    : isLinear_(false) {
    setLinearModelMedianErrorThreshold(0.25);
  }

  template <typename ParameterType, class DistanceFunction>
  arma::Col<double> LinearModelAnalysis<ParameterType, DistanceFunction>::getLinearModelEstimator() const noexcept {
    return linearModelEstimator_;
  }

  template <typename ParameterType, class DistanceFunction>
  void LinearModelAnalysis<ParameterType, DistanceFunction>::setLinearModelMedianErrorThreshold(
      const double& linearModelMedianErrorThreshold) {
    if(linearModelMedianErrorThreshold < 0 || linearModelMedianErrorThreshold > 1) {
      throw std::runtime_error("The quadratic model MeanError threshold (" + std::to_string(linearModelMedianErrorThreshold) + ") must be within 0 and 1.");
    }

    linearModelMedianErrorThreshold_ = linearModelMedianErrorThreshold;
  }

  template <typename ParameterType, class DistanceFunction>
  bool LinearModelAnalysis<ParameterType, DistanceFunction>::isLinear() const noexcept {
    return isLinear_;
  }
}
