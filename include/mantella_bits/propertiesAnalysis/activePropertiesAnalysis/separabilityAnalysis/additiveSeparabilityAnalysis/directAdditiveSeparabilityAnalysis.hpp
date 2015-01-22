namespace mant {
  class DirectAdditiveSeparabilityAnalysis : public AdditiveSeparabilityAnalysis<double, EuclideanDistance> {
    public:
      using AdditiveSeparabilityAnalysis<double, EuclideanDistance>::AdditiveSeparabilityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem)  override;
  };
}
