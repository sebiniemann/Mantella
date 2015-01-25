namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class FunctionModelAnalysis : public PassivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using PassivePropertiesAnalysis<ParameterType, DistanceFunction>::PassivePropertiesAnalysis;

      arma::Col<double> getResiduals() const noexcept;

    protected:
      arma::Col<double> residuals_;
  };

  //
  // Implementation
  //

  template <typename ParameterType, class DistanceFunction>
  arma::Col<double> FunctionModelAnalysis<ParameterType, DistanceFunction>::getResiduals() const noexcept {
    return residuals_;
  }
}
