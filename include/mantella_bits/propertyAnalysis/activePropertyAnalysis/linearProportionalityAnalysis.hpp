namespace mant {
  template <typename T>
  class LinearProportionalityAnalysis : public ActivePropertyAnalysis<T> {
    public:
      using ActivePropertyAnalysis<T>::ActivePropertyAnalysis;

    protected:
      void analyseImplementation(
          std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept override;
  };
}
