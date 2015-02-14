namespace mant {
  template <typename ParameterType>
  class ActivePropertyAnalysis : public PropertyAnalysis<ParameterType> {
    public:
      using PropertyAnalysis<ParameterType>::PropertyAnalysis;

      void analyse(
          OptimisationProblem<ParameterType>*) noexcept;

    protected:
      virtual void analyseImplementation(
          OptimisationProblem<ParameterType>*) noexcept = 0;
  };

  template <typename ParameterType>
  void ActivePropertyAnalysis<ParameterType>::analyse(
      OptimisationProblem<ParameterType>* optimisationProblem) noexcept {
    analyseImplementation(optimisationProblem);
  }
}
