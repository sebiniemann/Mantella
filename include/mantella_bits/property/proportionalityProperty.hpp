namespace mant {
  template <typename ParameterType>
  class ProportionalityAnalysis : public ActivePropertyAnalysis<ParameterType> {
    public:
      using ActivePropertyAnalysis<ParameterType>::ActivePropertyAnalysis;

      void setMaximalNumberOfIterations(
          const unsigned int& maximalNumberOfIterations) noexcept;

    protected:
      unsigned int maximalNumberOfIterations_;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  void ProportionalityAnalysis<ParameterType>::setMaximalNumberOfIterations(
      const unsigned int& maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }
}
