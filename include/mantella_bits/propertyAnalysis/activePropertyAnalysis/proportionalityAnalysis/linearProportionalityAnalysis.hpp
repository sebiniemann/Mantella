namespace mant {
  template <typename T>
  class LinearProportionalityAnalysis : public ProportionalityAnalysis<T> {
    public:
      using ProportionalityAnalysis<T>::ProportionalityAnalysis;

      LinearProportionalityProperty<T> getProperty() const noexcept;

    protected:
      LinearProportionalityProperty<T> property_;
  };

  //
  // Implementation
  //

  template <typename T>
  LinearProportionalityProperty<T> LinearProportionalityAnalysis<T>::getProperty() const noexcept {
    return property_;
  }
}
