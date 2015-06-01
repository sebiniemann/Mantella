namespace mant {
  template <typename T, typename U = double>
  class LinearProportionalityAnalysis : public ActivePropertyAnalysis<T, U> {
    public:
      using ActivePropertyAnalysis<T, U>::ActivePropertyAnalysis;

    protected:
      void analyseImplementation(
          std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept override;
  };
}
