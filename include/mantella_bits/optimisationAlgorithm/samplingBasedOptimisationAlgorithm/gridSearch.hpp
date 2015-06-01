namespace mant {
  template <typename T, typename U = double>
  class GridSearch : public SamplingBasedOptimisationAlgorithm<T, U> {
    public:
      explicit GridSearch(
          const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept;

      void setNumberOfSamples(
          const arma::Col<unsigned int>& numberOfSamples);

      std::string toString() const noexcept override;

    protected:
      arma::Col<unsigned int> numberOfSamples_;

      void optimiseImplementation() noexcept override;
  };

  //
  // Implementation
  //

  template <typename T, typename U>
  GridSearch<T, U>::GridSearch(
      const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept
    : SamplingBasedOptimisationAlgorithm<T, U>(optimisationProblem) {
    setNumberOfSamples(arma::zeros(this->numberOfDimensions_) + 10);
  }

  template <typename T, typename U>
  void GridSearch<T, U>::optimiseImplementation() noexcept {
    verify(arma::accu(numberOfSamples_) <= this->maximalNumberOfIterations_ * this->numberOfNodes_, "");
    if (std::is_integral<T>::value) {
      // For integral types, ensure that each dimension has as least the same amout of elements as
      // samples. 
      verify(arma::all(numberOfSamples_ >= this->getUpperBounds() - this->getLowerBounds()), "");
    }
    
    std::vector<arma::Col<T>> samples;
    for (std::size_t n = 0; n < this->numberOfDimensions_; ++n) {
      samples.push_back(arma::linspace<arma::Col<T>>(this->getLowerBounds()(n), this->getUpperBounds()(n), numberOfSamples_(n)));
    }

    arma::Col<unsigned int> sampleIndicies = arma::zeros<arma::Col<unsigned int>>(this->numberOfDimensions_);
    for(std::size_t n = 0; n < arma::accu(numberOfSamples_); ++n) {
      if (n % this->numberOfNodes_ == this->rank_) {
        ++this->numberOfIterations_;

        arma::Col<T> candidateParameter(this->numberOfDimensions_);
        for(std::size_t k = 0; k < sampleIndicies.n_elem; ++k) {
          candidateParameter(k) = samples.at(k)(sampleIndicies(k));
        }
        
        updateBestParameter(candidateParameter, this->getSoftConstraintsValue(candidateParameter), this->getObjectiveValue(candidateParameter));

        if (this->isFinished()) {
          break;
        }
      }
      
      ++sampleIndicies(0);
      for(std::size_t k = 0; k < sampleIndicies.n_elem - 1; ++k) {
        if(sampleIndicies(k) >= numberOfSamples_(k)) {
          sampleIndicies(k) = 0;
           ++sampleIndicies(k + 1);
        } else {
          break;
        }
      }
    }
  }

  template <typename T, typename U>
  void GridSearch<T, U>::setNumberOfSamples(
      const arma::Col<unsigned int>& numberOfSamples) {
    verify(arma::all(numberOfSamples > 0), "");

    numberOfSamples_ = numberOfSamples;
  }

  template <typename T, typename U>
  std::string GridSearch<T, U>::toString() const noexcept {
    return "grid_search";
  }
}
