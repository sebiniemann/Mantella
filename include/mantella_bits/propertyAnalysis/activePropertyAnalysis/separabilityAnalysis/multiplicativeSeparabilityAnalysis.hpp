namespace mant {
  template <typename ParameterType>
  class MultiplicativeSeparabilityAnalysis : public SeparabilityAnalysis<ParameterType> {
    public:
      using SeparabilityAnalysis<ParameterType>::SeparabilityAnalysis;

      MultiplicativeSeparabilityProperty<ParameterType> getProperty() const noexcept;

    protected:
      MultiplicativeSeparabilityProperty<ParameterType> property_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  MultiplicativeSeparabilityProperty<ParameterType> MultiplicativeSeparabilityAnalysis<ParameterType>::getProperty() const noexcept {
    return property_;
  }
}
