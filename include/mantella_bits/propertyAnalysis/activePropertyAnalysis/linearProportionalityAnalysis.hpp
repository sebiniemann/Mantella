namespace mant {
  template <typename T, typename U = double>
  class LinearProportionalityAnalysis : public ActivePropertyAnalysis<T, U> {
    public:
      using ActivePropertyAnalysis<T, U>::ActivePropertyAnalysis;

      std::string toString() const noexcept override;
      
    protected:
      void analyseImplementation(
          std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept override;
  };
  
  template <typename T, typename U>
  std::string LinearProportionalityAnalysis<T, U>::toString() const noexcept {
    return "linear_proportionality_analysis";
  }
}
