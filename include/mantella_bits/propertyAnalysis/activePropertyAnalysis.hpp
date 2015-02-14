namespace mant {
  template <typename ParameterType>
  class ActivePropertyAnalysis : public PropertyAnalysis<ParameterType> {
    public:
      using PropertyAnalysis<ParameterType>::PropertyAnalysis;

      void analyse(
          std::shared_ptr<OptimisationProblem<ParameterType>>) noexcept;

    protected:
      virtual void analyseImplementation(
          std::shared_ptr<OptimisationProblem<ParameterType>>) noexcept = 0;
  };

  template <typename ParameterType>
  void ActivePropertyAnalysis<ParameterType>::analyse(
      std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept {
    analyseImplementation(optimisationProblem);
  }
}
