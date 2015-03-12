namespace mant {
  template <typename ParameterType>
    class CorrelationAnalysis : public PassivePropertyAnalysis<ParameterType> {
    public:
      using PassivePropertyAnalysis<ParameterType>::PassivePropertyAnalysis;

      CorrelationProperty<ParameterType> getProperty() const noexcept;

    protected:
      CorrelationProperty<ParameterType> property_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  CorrelationProperty<ParameterType> CorrelationAnalysis<ParameterType>::getProperty() const noexcept {
    return property_;
  }
}
