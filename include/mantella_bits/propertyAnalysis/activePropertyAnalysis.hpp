namespace mant {
  template <typename T, typename U = double>
  class ActivePropertyAnalysis : public PropertyAnalysis<T, U> {
    
    public:
      using PropertyAnalysis<T, U>::PropertyAnalysis;

      void analyse(
          std::shared_ptr<OptimisationProblem<T, U>>) noexcept;

    protected:
      virtual void analyseImplementation(
          std::shared_ptr<OptimisationProblem<T, U>>) noexcept = 0;
  };

  template <typename T, typename U>
  void ActivePropertyAnalysis<T, U>::analyse(
      std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept {
    analyseImplementation(optimisationProblem);
  }
}
