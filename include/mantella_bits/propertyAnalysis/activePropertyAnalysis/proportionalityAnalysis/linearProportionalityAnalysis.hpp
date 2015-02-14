namespace mant {
  template <typename ParameterType>
  class LinearProportionalityAnalysis : public ProportionalityAnalysis<ParameterType> {
    public:
      using ProportionalityAnalysis<ParameterType>::ProportionalityAnalysis;

      LinearProportionalityProperty<ParameterType> getProperty() const noexcept;

    protected:
      LinearProportionalityProperty<ParameterType> property_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  LinearProportionalityProperty<ParameterType> LinearProportionalityAnalysis<ParameterType>::getProperty() const noexcept {
    return property_;
  }
}
