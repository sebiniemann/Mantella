namespace mant {
  template <typename T = double>
  class ActivePropertyAnalysis : public PropertyAnalysis<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
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
