namespace mant {
  template <typename T>
  class LipschitzContinuityAnalysis : public ContinuityAnalysis<T> {
    public:
      using ContinuityAnalysis<T>::ContinuityAnalysis;

      LipschitzContinuityProperty<T> getProperty() const noexcept;

    protected:
      LipschitzContinuityProperty<T> property_;
  };

  //
  // Implementation
  //

  template <typename T>
  LipschitzContinuityProperty<T> LipschitzContinuityAnalysis<T>::getProperty() const noexcept {
    return property_;
  }
}
