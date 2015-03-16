namespace mant {
  template <typename ParameterType>
  class GridSearch : public SamplingBasedOptimisationAlgorithm<ParameterType> {
    public:
      explicit GridSearch(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      void setSamplingFactors(
          const arma::Col<double> samplingFactors);

      std::string toString() const noexcept override;

    protected:
      arma::Col<double> samplingFactors_;

      void optimiseImplementation() noexcept override;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  GridSearch<ParameterType>::GridSearch(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : SamplingBasedOptimisationAlgorithm<ParameterType>(optimisationProblem) {
    setSamplingFactors(arma::ones(this->numberOfDimensions_) / static_cast<double>(this->numberOfDimensions_));
  }

  template <typename ParameterType>
  void GridSearch<ParameterType>::optimiseImplementation() noexcept {
    const arma::Col<double>& scaledSamplingFactors = samplingFactors_(0) / samplingFactors_;
    const arma::Col<unsigned int>& numberOfSamples_ = arma::conv_to<arma::Col<unsigned int>>::from(scaledSamplingFactors * std::pow(this->maximalNumberOfIterations_ / arma::prod(scaledSamplingFactors), 1.0 / static_cast<double>(this->numberOfDimensions_)));

    std::vector<arma::Col<double>> sampleParameters_;
    for (std::size_t n = 0; n < this->numberOfDimensions_; ++n) {
      sampleParameters_.push_back(arma::linspace(this->getLowerBounds()(n), this->getUpperBounds()(n), numberOfSamples_(n)));
    }

    arma::Col<unsigned int> sampleIndicies_ = arma::zeros<arma::Col<unsigned int>>(sampleParameters_.size());
    arma::Col<double> candidateParameter(this->numberOfDimensions_);

    const unsigned int& overallNumberOfSamples = arma::sum(numberOfSamples_);
    for(unsigned int n = 0; n < overallNumberOfSamples; ++n) {
      ++this->numberOfIterations_;

      for(std::size_t k = 0; k < sampleIndicies_.n_elem; ++k) {
        candidateParameter(k) = sampleParameters_.at(k)(sampleIndicies_(k));
      }

      ++sampleIndicies_(0);
      for(std::size_t k = 0; k < sampleIndicies_.n_elem - 1; ++k) {
        if(sampleIndicies_(k) >= numberOfSamples_(k)) {
          sampleIndicies_(k) = 0;
           ++sampleIndicies_(k + 1);
        }
      }

      const double& candidateSoftConstraintsValue = this->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = this->getObjectiveValue(candidateParameter);

      if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
        this->bestParameter_ = candidateParameter;
        this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
        this->bestObjectiveValue_ = candidateObjectiveValue;
      }

      if (this->isFinished() || this->isTerminated()) {
        break;
      }
    }
  }

  template <typename ParameterType>
  void GridSearch<ParameterType>::setSamplingFactors(
      const arma::Col<double> samplingFactors) {
    if(samplingFactors.n_elem != this->numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the sampling factors (" + std::to_string(samplingFactors.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->numberOfDimensions_) + ").");
    } else if(arma::sum(samplingFactors) != 1.0) {
      throw std::logic_error("The sum of all sampling factors (" + std::to_string(arma::sum(samplingFactors)) + ") must be 1.");
    }

    samplingFactors_ = samplingFactors;
  }

  template <typename ParameterType>
  std::string GridSearch<ParameterType>::toString() const noexcept {
    return "GridSearch";
  }
}
