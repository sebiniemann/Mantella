namespace mant {
  template <typename ParameterType>
  class LipschitzContinuityAnalysis : public ContinuityAnalysis<ParameterType> {
    public:
      using ContinuityAnalysis<ParameterType>::ContinuityAnalysis;

      LipschitzContinuityProperty<ParameterType> getProperty() const noexcept;

    protected:
      LipschitzContinuityProperty<ParameterType> property_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  LipschitzContinuityProperty<ParameterType> LipschitzContinuityAnalysis<ParameterType>::getProperty() const noexcept {
    return property_;
  }
}
