namespace mant {
  template <typename ParameterType>
  class ProportionalityAnalysis : public ActivePropertiesAnalysis<ParameterType> {
    public:
      using ActivePropertiesAnalysis<ParameterType>::ActivePropertiesAnalysis;

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
