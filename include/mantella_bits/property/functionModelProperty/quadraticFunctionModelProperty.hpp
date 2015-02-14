namespace mant {
  class QuadraticFunctionModelProperty : public FunctionModelProperty<double> {
    public:
      using FunctionModelProperty<double>::FunctionModelProperty;

      inline arma::Mat<double> getCoefficients() const noexcept;

      inline void setCoefficients(
          const arma::Mat<double> coefficients);

    protected:
      arma::Mat<double> coefficients_;
  };

  //
  // Implementation
  //

  arma::Mat<double> QuadraticFunctionModelProperty::getCoefficients() const noexcept {
    return coefficients_;
  }

  void QuadraticFunctionModelProperty::setCoefficients(
      const arma::Mat<double> coefficients) {
    if(!coefficients.is_square()) {
      // TODO Throw an exception
    }

    coefficients_ = coefficients;
  }
}
