namespace mant {
  template <typename ParameterType>
  class LinearProportionalityProperty : public ProportionalityProperty<ParameterType> {
    public:
      using ProportionalityProperty<ParameterType>::ProportionalityProperty;

      bool isProportional() const noexcept;

      void isProportional(const bool isProportional) noexcept;

      std::unordered_map<unsigned int, std::pair<double, arma::Col<unsigned int>>> getProportions() const noexcept;

      void setProportions(
          const std::unordered_map<unsigned int, std::pair<double, arma::Col<unsigned int>>> proportions);

    protected:
      bool isProportional_;

      std::unordered_map<unsigned int, std::pair<double, arma::Col<unsigned int>>> proportions_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  bool LinearProportionalityProperty<ParameterType>::isProportional() const noexcept {
    return isProportional_;
  }

  template <typename ParameterType>
  void LinearProportionalityProperty<ParameterType>::isProportional(const bool isProportional) noexcept {
    isProportional_ = isProportional;
  }

  template <typename ParameterType>
  std::unordered_map<unsigned int, std::pair<double, arma::Col<unsigned int>>> LinearProportionalityProperty<ParameterType>::getProportions() const noexcept {
    return proportions_;
  }

  template <typename ParameterType>
  void LinearProportionalityProperty<ParameterType>::setProportions(
      std::unordered_map<unsigned int, std::pair<double, arma::Col<unsigned int>>> proportions) {
    proportions_ = proportions;
  }
}
