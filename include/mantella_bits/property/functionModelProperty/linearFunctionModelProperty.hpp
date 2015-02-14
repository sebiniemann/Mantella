namespace mant {
  class LinearFunctionModelProperty : public FunctionModelProperty<double> {
    public:
      using FunctionModelProperty<double>::FunctionModelProperty;

      inline arma::Col<double> getCoefficients() const noexcept;

      inline void setCoefficients(
          const arma::Col<double> coefficients) noexcept;

    protected:
      arma::Col<double> coefficients_;
  };

  //
  // Implementation
  //

  arma::Col<double> LinearFunctionModelProperty::getCoefficients() const noexcept {
    return coefficients_;
  }

  void LinearFunctionModelProperty::setCoefficients(
      const arma::Col<double> coefficients) noexcept {
    coefficients_ = coefficients;
  }
}
