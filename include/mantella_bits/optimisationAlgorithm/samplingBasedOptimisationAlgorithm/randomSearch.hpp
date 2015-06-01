namespace mant {
  template <typename T, typename U = double>
  class RandomSearch : public SamplingBasedOptimisationAlgorithm<T, U> {
    public:
      using SamplingBasedOptimisationAlgorithm<T, U>::SamplingBasedOptimisationAlgorithm;

      std::string toString() const noexcept override;

    protected:
      void optimiseImplementation() noexcept override;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  void RandomSearch<T, U>::optimiseImplementation() noexcept {
    while(!this->isFinished() && !this->isTerminated()) {
      ++this->numberOfIterations_;
    
      const arma::Col<T>& candidateParameter = this->getRandomParameter();
      updateBestParameter(candidateParameter, this->getSoftConstraintsValue(candidateParameter), this->getObjectiveValue(candidateParameter));
    };
  }

  template <typename T, typename U>
  std::string RandomSearch<T, U>::toString() const noexcept {
    return "random_search";
  }
}
