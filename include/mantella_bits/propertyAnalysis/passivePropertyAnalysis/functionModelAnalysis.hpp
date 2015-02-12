namespace mant {
  template <typename ParameterType>
  class FunctionModelAnalysis : public PassivePropertyAnalysis<ParameterType> {
    public:
      using PassivePropertyAnalysis<ParameterType>::PassivePropertyAnalysis;

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
