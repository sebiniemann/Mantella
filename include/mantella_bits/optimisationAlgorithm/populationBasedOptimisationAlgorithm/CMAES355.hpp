namespace mant {

  template <typename T>
  class CMAES355 : public PopulationBasedOptimisationAlgorithm<T> {
  public:
    explicit CMAES355(
        const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
        const unsigned int populationSize) noexcept;

    std::string toString() const noexcept override;

  protected:

    void optimiseImplementation() override;
  };
  
  template <typename T>
  CMAES355<T>::CMAES355(const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : PopulationBasedOptimisationAlgorithm<T>(optimisationProblem, populationSize) {
    
  }
  
  template <typename T>
  void CMAES355<T>::optimiseImplementation() {
    
  }

  template <typename T>
  std::string CMAES355<T>::toString() const noexcept {
    return "CMAES355";
  }
}