namespace mant {
  template <typename T = double>
  class RandomSearch : public SamplingBasedOptimisationAlgorithm<T> {
    public:
      using SamplingBasedOptimisationAlgorithm<T>::SamplingBasedOptimisationAlgorithm;

      std::string toString() const noexcept override;

    protected:
      void optimiseImplementation() noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  void RandomSearch<T>::optimiseImplementation() noexcept {
    do {
      ++this->numberOfIterations_;
    
      const arma::Col<T>& candidateParameter = this->getRandomParameter();

      this->updateBestParameter(candidateParameter, this->getSoftConstraintsValue(candidateParameter), this->getObjectiveValue(candidateParameter));
    } while(!this->isFinished() && !this->isTerminated());
  }

  template <typename T>
  std::string RandomSearch<T>::toString() const noexcept {
    return "random_search";
  }
}
