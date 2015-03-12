namespace mant {
  template <typename ParameterType>
  class AdditiveSeparabilityAnalysis : public SeparabilityAnalysis<ParameterType> {
    public:
      using SeparabilityAnalysis<ParameterType>::SeparabilityAnalysis;

      AdditiveSeparabilityProperty<ParameterType> getProperty() const noexcept;

    protected:
      AdditiveSeparabilityProperty<ParameterType> property_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  AdditiveSeparabilityProperty<ParameterType> AdditiveSeparabilityAnalysis<ParameterType>::getProperty() const noexcept {
    return property_;
  }
}
