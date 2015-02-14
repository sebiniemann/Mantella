namespace mant {
  class DirectLinearProportionalityAnalysis : public LinearProportionalityAnalysis<double> {
    public:
      using LinearProportionalityAnalysis<double>::LinearProportionalityAnalysis;

    protected:
      void analyseImplementation(
          const OptimisationProblem<double>* optimisationProblem) noexcept override;
  };
}
