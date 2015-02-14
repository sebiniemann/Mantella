namespace mant {
  class LinearFunctionModel : public Property<double> {
    public:
      inline arma::Col<double> getCoefficents() const noexcept;

      inline void setCoefficents(
          const arma::Col<double>& coefficents) noexcept;

    protected:
      arma::Col<double> coefficents_;
  };

  //
  // Implementation
  //

  arma::Col<double> LinearFunctionModel::getCoefficents() const noexcept {
    return coefficents_;
  }

  void LinearFunctionModel::setCoefficents(
      const arma::Col<double>& coefficents) noexcept {
    coefficents_ = coefficents;
  }
}
