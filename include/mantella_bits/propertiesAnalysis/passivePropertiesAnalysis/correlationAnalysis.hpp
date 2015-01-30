namespace mant {
  template <typename ParameterType>
    class CorrelationAnalysis : public PassivePropertiesAnalysis<ParameterType> {
    public:
      explicit CorrelationAnalysis() noexcept;

      CorrelationAnalysis(const CorrelationAnalysis&) = delete;
      CorrelationAnalysis& operator=(const CorrelationAnalysis&) = delete;

      double getCorrelationCoefficient() const noexcept;

    protected:
      double correlationCoefficient_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  CorrelationAnalysis<ParameterType>::CorrelationAnalysis() noexcept
    : correlationCoefficient_(0.0) {

  }

  template <typename ParameterType>
  double CorrelationAnalysis<ParameterType>::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }
}
