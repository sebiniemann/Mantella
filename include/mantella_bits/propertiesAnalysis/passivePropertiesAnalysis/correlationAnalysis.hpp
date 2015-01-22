namespace mant {
  template <typename ParameterType, class DistanceFunction>
    class CorrelationAnalysis : public PassivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      explicit CorrelationAnalysis() ;

      CorrelationAnalysis(const CorrelationAnalysis&) = delete;
      CorrelationAnalysis& operator=(const CorrelationAnalysis&) = delete;

      double getCorrelationCoefficient() const ;

    protected:
      double correlationCoefficient_;
  };

  template <typename ParameterType, class DistanceFunction>
  CorrelationAnalysis<ParameterType, DistanceFunction>::CorrelationAnalysis() 
    : correlationCoefficient_(0.0) {

  }

  template <typename ParameterType, class DistanceFunction>
  double CorrelationAnalysis<ParameterType, DistanceFunction>::getCorrelationCoefficient() const  {
    return correlationCoefficient_;
  }
}
