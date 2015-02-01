namespace mant {
  template <typename ParameterType>
  class LipschitzContinuityAnalysis : public ContinuityAnalysis<ParameterType> {
    public:
      explicit LipschitzContinuityAnalysis() noexcept;

      double getLipschitzConstant() const noexcept;

    protected:
      double lipschitzConstant_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  LipschitzContinuityAnalysis<ParameterType>::LipschitzContinuityAnalysis() noexcept
    : lipschitzConstant_(0.0) {

  }

  template <typename ParameterType>
  double LipschitzContinuityAnalysis<ParameterType>::getLipschitzConstant() const noexcept {
    return lipschitzConstant_;
  }
}
