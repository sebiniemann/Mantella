namespace mant {
  class LinearProportionalityAnalysis : public ActivePropertyAnalysis<double> {
    public:
      using ActivePropertyAnalysis<double>::ActivePropertyAnalysis;

    protected:
      void analyseImplementation(
          std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };
}
