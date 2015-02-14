namespace mant {
  template <typename ParameterType>
  class MultiplicativeSeparabilityProperty : SeparabilityProperty<ParameterType> {
    public:
      using SeparabilityProperty<ParameterType>::SeparabilityProperty;

      bool isSeparable() const noexcept;

      void isSeparable(const bool isSeparable) noexcept;

      std::vector<arma::Col<unsigned int>> getSeparation() const noexcept;

      void setSeparation(
          const std::vector<arma::Col<unsigned int>> separation);

    protected:
      bool isSeparable_;

      std::vector<arma::Col<unsigned int>> separation_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  bool MultiplicativeSeparabilityProperty<ParameterType>::isSeparable() const noexcept {
    return isSeparable_;
  }

  template <typename ParameterType>
  void MultiplicativeSeparabilityProperty<ParameterType>::isSeparable(const bool isSeparable) noexcept {
    isSeparable_ = isSeparable;
  }

  template <typename ParameterType>
  std::vector<arma::Col<unsigned int>> MultiplicativeSeparabilityProperty<ParameterType>::getSeparation() const noexcept {
    return separation_;
  }

  template <typename ParameterType>
  void MultiplicativeSeparabilityProperty<ParameterType>::setSeparation(
      const std::vector<arma::Col<unsigned int>> separation) {
    separation_ = separation;
  }
}
