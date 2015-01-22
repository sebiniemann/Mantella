namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class GridSearch : public SamplingBasedAlgorithm<ParameterType, DistanceFunction> {
    public:
      explicit GridSearch(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) ;

      GridSearch(const GridSearch&) = delete;
      GridSearch& operator=(const GridSearch&) = delete;

      void setSamplingFactors(
          const arma::Col<double>& samplingFactors);

      std::string to_string() const  override;

    protected:
      arma::Col<double> samplingFactors_;

      void optimiseImplementation()  override;
  };

  template <typename ParameterType, class DistanceFunction>
  GridSearch<ParameterType, DistanceFunction>::GridSearch(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) 
    : SamplingBasedAlgorithm<ParameterType, DistanceFunction>(optimisationProblem) {
    setSamplingFactors(arma::ones(this->optimisationProblem_->getNumberOfDimensions()) / static_cast<double>(this->optimisationProblem_->getNumberOfDimensions()));
  }

  template <typename ParameterType, class DistanceFunction>
  void GridSearch<ParameterType, DistanceFunction>::optimiseImplementation()  {
    const arma::Col<double>& scaledSamplingFactors = samplingFactors_.at(0) / samplingFactors_;
    const arma::Col<unsigned int>& numberOfSamples_ = arma::conv_to<arma::Col<unsigned int>>::from(scaledSamplingFactors * std::pow(this->maximalNumberOfIterations_ / arma::prod(scaledSamplingFactors), 1.0 / static_cast<double>(this->optimisationProblem_->getNumberOfDimensions())));

    std::vector<arma::Col<double>> sampleParameters_;
    for (std::size_t n = 0; n < this->optimisationProblem_->getNumberOfDimensions(); ++n) {
      sampleParameters_.push_back(arma::linspace(this->optimisationProblem_->getLowerBounds().at(n), this->optimisationProblem_->getUpperBounds().at(n), numberOfSamples_.at(n)));
    }

    arma::Col<unsigned int> sampleIndicies_ = arma::zeros<arma::Col<unsigned int>>(sampleParameters_.size());
    arma::Col<double> candidateParameter(this->optimisationProblem_->getNumberOfDimensions());

    const unsigned int& overallNumberOfSamples = arma::sum(numberOfSamples_);
    for(unsigned int n = 0; n < overallNumberOfSamples; ++n) {
      ++this->numberOfIterations_;

      for(std::size_t k = 0; k < sampleIndicies_.n_elem; ++k) {
        candidateParameter.at(k) = sampleParameters_.at(k).at(sampleIndicies_.at(k));
      }

      ++sampleIndicies_.at(0);
      for(std::size_t k = 0; k < sampleIndicies_.n_elem - 1; ++k) {
        if(sampleIndicies_.at(k) >= numberOfSamples_.at(k)) {
          sampleIndicies_.at(k) = 0;
           ++sampleIndicies_.at(k + 1);
        }
      }

      const double& candidateSoftConstraintsValue = this->optimisationProblem_->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = this->optimisationProblem_->getObjectiveValue(candidateParameter);

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

  template <typename ParameterType, class DistanceFunction>
  void GridSearch<ParameterType, DistanceFunction>::setSamplingFactors(
      const arma::Col<double>& samplingFactors) {
    if(samplingFactors.n_elem != this->optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the sampling factors (" + std::to_string(samplingFactors.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if(arma::sum(samplingFactors) != 1.0) {
      throw std::logic_error("The sum of all sampling factors (" + std::to_string(arma::sum(samplingFactors)) + ") must be 1.");
    }

    samplingFactors_ = samplingFactors;
  }

  template <typename ParameterType, class DistanceFunction>
  std::string GridSearch<ParameterType, DistanceFunction>::to_string() const  {
    return "GridSearch";
  }
}
