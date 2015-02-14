namespace mant {
  template <typename ParameterType>
  class ActivePropertyAnalysis : public PropertyAnalysis<ParameterType> {
    public:
      using PropertyAnalysis<ParameterType>::PropertyAnalysis;

      void analyse(
          const OptimisationProblem<ParameterType>*) noexcept;

    protected:
      virtual void analyseImplementation(
          const OptimisationProblem<ParameterType>*) noexcept = 0;
  };

  template <typename ParameterType>
  void ActivePropertyAnalysis<ParameterType>::analyse(
      const OptimisationProblem<ParameterType>* optimisationProblem) noexcept {
    analyseImplementation(optimisationProblem);
  }
}
