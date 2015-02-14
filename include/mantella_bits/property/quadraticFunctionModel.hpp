namespace mant {
  class QuadraticFunctionModel : public Property<double> {
    public:
      inline arma::Mat<double> getCoefficents() const noexcept;

      inline void setCoefficents(
          const arma::Mat<double>& coefficents);

    protected:
      arma::Mat<double> coefficents_;
  };

  //
  // Implementation
  //

  arma::Mat<double> QuadraticFunctionModel::getCoefficents() const noexcept {
    return coefficents_;
  }

  void QuadraticFunctionModel::setCoefficents(
      const arma::Mat<double>& coefficents) {
    if(!coefficents.is_square()) {
      // TODO Throw an exception
    }

    coefficents_ = coefficents;
  }
}
