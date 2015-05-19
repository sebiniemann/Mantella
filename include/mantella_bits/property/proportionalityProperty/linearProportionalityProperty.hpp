namespace mant {
  template <typename T>
  class LinearProportionalityProperty : public ProportionalityProperty<T> {
    public:
      using ProportionalityProperty<T>::ProportionalityProperty;

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

  template <typename T>
  bool LinearProportionalityProperty<T>::isProportional() const noexcept {
    return isProportional_;
  }

  template <typename T>
  void LinearProportionalityProperty<T>::isProportional(const bool isProportional) noexcept {
    isProportional_ = isProportional;
  }

  template <typename T>
  std::unordered_map<unsigned int, std::pair<double, arma::Col<unsigned int>>> LinearProportionalityProperty<T>::getProportions() const noexcept {
    return proportions_;
  }

  template <typename T>
  void LinearProportionalityProperty<T>::setProportions(
      std::unordered_map<unsigned int, std::pair<double, arma::Col<unsigned int>>> proportions) {
    proportions_ = proportions;
  }
}
