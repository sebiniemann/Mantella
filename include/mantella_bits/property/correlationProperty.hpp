namespace mant {
  template <typename ParameterType>
  class CorrelationProperty : public Property<ParameterType> {
    public:
      double getCorrelationCoefficient() const noexcept;

      void setCorrelationCoefficient(
          const double correlationCoefficient) noexcept;

    protected:
      double correlationCoefficient_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  double CorrelationProperty<ParameterType>::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }

  template <typename ParameterType>
  void CorrelationProperty<ParameterType>::setCorrelationCoefficient(
      const double correlationCoefficient) noexcept {
    correlationCoefficient_ = correlationCoefficient;
  }
}
