namespace mant {
  template <typename T>
  class ActivePropertyAnalysis : public PropertyAnalysis<T> {
    public:
      using PropertyAnalysis<T>::PropertyAnalysis;

      void analyse(
          std::shared_ptr<OptimisationProblem<T>>) noexcept;

    protected:
      virtual void analyseImplementation(
          std::shared_ptr<OptimisationProblem<T>>) noexcept = 0;
  };

  template <typename T>
  void ActivePropertyAnalysis<T>::analyse(
      std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept {
    analyseImplementation(optimisationProblem);
  }
}
