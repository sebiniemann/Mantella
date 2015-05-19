namespace mant {
  template <typename T>
  class SeparabilityAnalysis : public ActivePropertyAnalysis<T> {
    public:
      explicit SeparabilityAnalysis() noexcept
        : maximalNumberOfIterations_(1000.0) {

      }

      void setMaximalNumberOfIterations(
          const unsigned int maximalNumberOfIterations) noexcept;

    protected:
      unsigned int maximalNumberOfIterations_;
  };

  //
  // Implementation
  //

  template <typename T>
  void SeparabilityAnalysis<T>::setMaximalNumberOfIterations(
      const unsigned int maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }
}
