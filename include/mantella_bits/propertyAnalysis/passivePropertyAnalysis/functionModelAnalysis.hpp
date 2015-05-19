namespace mant {
  template <typename T>
  class FunctionModelAnalysis : public PassivePropertyAnalysis<T> {
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      arma::Col<double> getResiduals() const noexcept;

    protected:
      arma::Col<double> residuals_;
  };

  //
  // Implementation
  //

  template <typename T>
  arma::Col<double> FunctionModelAnalysis<T>::getResiduals() const noexcept {
    return residuals_;
  }
}
