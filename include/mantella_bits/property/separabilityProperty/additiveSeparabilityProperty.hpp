namespace mant {
  template <typename T>
  class AdditiveSeparabilityProperty : public SeparabilityProperty<T> {
    public:
      using SeparabilityProperty<T>::SeparabilityProperty;

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

  template <typename T>
  bool AdditiveSeparabilityProperty<T>::isSeparable() const noexcept {
    return isSeparable_;
  }

  template <typename T>
  void AdditiveSeparabilityProperty<T>::isSeparable(const bool isSeparable) noexcept {
    isSeparable_ = isSeparable;
  }

  template <typename T>
  std::vector<arma::Col<unsigned int>> AdditiveSeparabilityProperty<T>::getSeparation() const noexcept {
    return separation_;
  }

  template <typename T>
  void AdditiveSeparabilityProperty<T>::setSeparation(
      const std::vector<arma::Col<unsigned int>> separation) {
    separation_ = separation;
  }
}
