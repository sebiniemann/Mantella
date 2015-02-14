namespace mant {
  class DirectLinearProportionalityAnalysis : public LinearProportionalityAnalysis<double> {
    public:
      using LinearProportionalityAnalysis<double>::LinearProportionalityAnalysis;

    protected:
      void analyseImplementation(
          OptimisationProblem<double>* optimisationProblem) noexcept override;
  };
}
