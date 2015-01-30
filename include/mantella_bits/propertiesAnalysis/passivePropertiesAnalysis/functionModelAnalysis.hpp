namespace mant {
  template <typename ParameterType>
  class FunctionModelAnalysis : public PassivePropertiesAnalysis<ParameterType> {
    public:
      using PassivePropertiesAnalysis<ParameterType>::PassivePropertiesAnalysis;

      arma::Col<double> getResiduals() const noexcept;

    protected:
      arma::Col<double> residuals_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  arma::Col<double> FunctionModelAnalysis<ParameterType>::getResiduals() const noexcept {
    return residuals_;
  }
}
