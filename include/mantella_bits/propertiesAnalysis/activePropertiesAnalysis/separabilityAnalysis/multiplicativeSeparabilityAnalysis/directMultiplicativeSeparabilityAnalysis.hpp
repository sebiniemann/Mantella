namespace mant {
  class DirectMultiplicativeSeparabilityAnalysis : public MultiplicativeSeparabilityAnalysis<double, EuclideanDistance> {
    public:
      using MultiplicativeSeparabilityAnalysis<double, EuclideanDistance>::MultiplicativeSeparabilityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem)  override;
  };
}
