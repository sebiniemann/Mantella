namespace mant {
  class DirectLinearProportionalityAnalysis : public LinearProportionalityAnalysis<double, EuclideanDistance> {
    public:
      using LinearProportionalityAnalysis<double, EuclideanDistance>::LinearProportionalityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem)  override;
  };
}
