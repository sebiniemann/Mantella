namespace mant {
  template <typename T>
  class CorrelationProperty : public Property<T> {
    public:
      double getCorrelationCoefficient() const noexcept;

      void setCorrelationCoefficient(
          const double correlationCoefficient) noexcept;

    protected:
      double correlationCoefficient_;
  };

  //
  // Implementation
  //

  template <typename T>
  double CorrelationProperty<T>::getCorrelationCoefficient() const noexcept {
    return correlationCoefficient_;
  }

  template <typename T>
  void CorrelationProperty<T>::setCorrelationCoefficient(
      const double correlationCoefficient) noexcept {
    correlationCoefficient_ = correlationCoefficient;
  }
}
