namespace mant {
  template <typename T>
    class CorrelationAnalysis : public PassivePropertyAnalysis<T> {
    public:
      using PassivePropertyAnalysis<T>::PassivePropertyAnalysis;

      CorrelationProperty<T> getProperty() const noexcept;

    protected:
      CorrelationProperty<T> property_;
  };

  //
  // Implementation
  //

  template <typename T>
  CorrelationProperty<T> CorrelationAnalysis<T>::getProperty() const noexcept {
    return property_;
  }
}
