namespace mant {
  template <typename T>
  class ProportionalityAnalysis : public ActivePropertyAnalysis<T> {
    public:
      using ActivePropertyAnalysis<T>::ActivePropertyAnalysis;

      void setMaximalNumberOfIterations(
          const unsigned int maximalNumberOfIterations) noexcept;

    protected:
      unsigned int maximalNumberOfIterations_;
  };

  //
  // Implementation
  //

  template <typename T>
  void ProportionalityAnalysis<T>::setMaximalNumberOfIterations(
      const unsigned int maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }
}
