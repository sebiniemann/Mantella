namespace mant {
  template <typename ParameterType>
  class Correlation : public Property<ParameterType> {
    public:
      double getCorrelation() const noexcept;

      void setCorrelation(
          const double& correlation) noexcept;

    protected:
      double correlation_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  double Correlation<ParameterType>::getCorrelation() const noexcept {
    return correlation_;
  }

  template <typename ParameterType>
  void Correlation<ParameterType>::setCorrelation(
      const double& correlation) noexcept {
    correlation_ = correlation;
  }
}
