namespace mant {
  template <typename T>
  class MultiplicativeSeparabilityAnalysis : public SeparabilityAnalysis<T> {
    public:
      using SeparabilityAnalysis<T>::SeparabilityAnalysis;

      MultiplicativeSeparabilityProperty<T> getProperty() const noexcept;

    protected:
      MultiplicativeSeparabilityProperty<T> property_;
  };

  //
  // Implementation
  //

  template <typename T>
  MultiplicativeSeparabilityProperty<T> MultiplicativeSeparabilityAnalysis<T>::getProperty() const noexcept {
    return property_;
  }
}
