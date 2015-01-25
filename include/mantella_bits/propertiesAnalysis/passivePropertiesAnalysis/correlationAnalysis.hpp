namespace mant {
  template <typename ParameterType, class DistanceFunction>
    class CorrelationAnalysis : public PassivePropertiesAnalysis<ParameterType, DistanceFunction> {
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

  template <typename ParameterType, class DistanceFunction>
  CorrelationAnalysis<ParameterType, DistanceFunction>::CorrelationAnalysis() noexcept
    : correlationCoefficient_(0.0) {

  }

  template <typename ParameterType, class DistanceFunction>
  double CorrelationAnalysis<ParameterType, DistanceFunction>::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }
}
