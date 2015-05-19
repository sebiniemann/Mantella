namespace mant {
  template <typename T>
  class AdditiveSeparabilityAnalysis : public SeparabilityAnalysis<T> {
    public:
      using SeparabilityAnalysis<T>::SeparabilityAnalysis;

      AdditiveSeparabilityProperty<T> getProperty() const noexcept;

    protected:
      AdditiveSeparabilityProperty<T> property_;
  };

  //
  // Implementation
  //

  template <typename T>
  AdditiveSeparabilityProperty<T> AdditiveSeparabilityAnalysis<T>::getProperty() const noexcept {
    return property_;
  }
}
