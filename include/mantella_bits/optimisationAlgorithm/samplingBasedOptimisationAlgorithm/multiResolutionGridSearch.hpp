namespace mant {
  template <typename T>
  class MultiResolutionGridSearch : public SamplingBasedOptimisationAlgorithm<T> {
    public:
      explicit MultiResolutionGridSearch(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept;

      void setMinimalSamplingDistances(
          const arma::Col<double> minimalSamplingDistances);

      void setMaximalSamplesPerResolution(
          const unsigned int maximalSamplesPerResolution) noexcept;

      void setSamplingDistributionPerDimension(
          const arma::Col<double> samplingDistributionPerDimension);

      std::string toString() const noexcept override;

    protected:
      arma::Col<T> minimalSamplingDistances_;
      unsigned int maximalSamplesPerResolution_;
      arma::Col<double> samplingDistributionPerDimension_;

      void optimiseImplementation() noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  MultiResolutionGridSearch<T>::MultiResolutionGridSearch(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept
    : SamplingBasedOptimisationAlgorithm<T>(optimisationProblem) {
    setMinimalSamplingDistances(arma::ones(this->numberOfDimensions_) * 1e-3);
    setSamplingDistributionPerDimension(arma::ones(this->numberOfDimensions_) / static_cast<double>(this->numberOfDimensions_));
    setMaximalSamplesPerResolution(11);
  }

  template <typename T>
  void MultiResolutionGridSearch<T>::optimiseImplementation() noexcept {
    unsigned int resolutionDepth = 0;

    std::map<unsigned int, std::unordered_map<arma::Col<double>, std::pair<double, double>, Hash, IsEqual>> samplesPerResolutions;
    samplesPerResolutions.insert({resolutionDepth, std::unordered_map<arma::Col<double>, std::pair<double, double>, Hash, IsEqual>()});

    double gridSoftCostraintsValueThreshold = std::numeric_limits<double>::infinity();
    double gridObjectiveValueThreshold = std::numeric_limits<double>::infinity();

    arma::Col<double> gridLowerBounds = this->getLowerBounds();
    arma::Col<double> gridUpperBounds = this->getUpperBounds();

    arma::Col<unsigned int> belowLowerBound(this->numberOfDimensions_, arma::fill::zeros);
    arma::Col<unsigned int> aboveUpperBound(this->numberOfDimensions_, arma::fill::zeros);
    while(!this->isFinished() & !this->isTerminated()) {
      const arma::Col<double>& scaledSamplingFactors = samplingDistributionPerDimension_(0) / samplingDistributionPerDimension_;
      const arma::Col<unsigned int>& numberOfSamples = arma::conv_to<arma::Col<unsigned int>>::from(scaledSamplingFactors * std::pow(std::pow(maximalSamplesPerResolution_ / std::pow(2, arma::sum(belowLowerBound) + arma::sum(aboveUpperBound)), this->numberOfDimensions_) / arma::prod(scaledSamplingFactors), 1.0 / static_cast<double>(this->numberOfDimensions_)));

      std::vector<arma::Col<double>> sampleParameters_;
      for (std::size_t n = 0; n < this->numberOfDimensions_; ++n) {
        sampleParameters_.push_back(arma::linspace(gridLowerBounds(n), gridUpperBounds(n), numberOfSamples(n)));
      }

      arma::Col<unsigned int> sampleIndicies_ = arma::zeros<arma::Col<unsigned int>>(sampleParameters_.size());
      arma::Col<double> candidateParameter(this->numberOfDimensions_);

      const unsigned int& overallNumberOfSamples = arma::prod(numberOfSamples);
      for(unsigned int n = 0; n < overallNumberOfSamples; ++n) {
        ++this->numberOfIterations_;

        for(std::size_t k = 0; k < sampleIndicies_.n_elem; ++k) {
          candidateParameter(k) = sampleParameters_.at(k)(sampleIndicies_(k));
        }

        ++sampleIndicies_(0);
        for(std::size_t k = 0; k < sampleIndicies_.n_elem - 1; ++k) {
          if(sampleIndicies_(k) >= numberOfSamples(k)) {
            sampleIndicies_(k) = 0;
             ++sampleIndicies_(k + 1);
          }
        }

        const double& candidateSoftConstraintsValue = this->getSoftConstraintsValue(candidateParameter);
        const double& candidateObjectiveValue = this->getObjectiveValue(candidateParameter);

        samplesPerResolutions.at(resolutionDepth).insert({candidateParameter, {candidateSoftConstraintsValue, candidateObjectiveValue}});

        if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
          this->bestParameter_ = candidateParameter;
          this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
          this->bestObjectiveValue_ = candidateObjectiveValue;
        }

        if(this->isFinished() || this->isTerminated()) {
          break;
        }
      }

      arma::Col<double> bestGridParameter;
      double bestGridSoftConstraintsValue = std::numeric_limits<double>::infinity();
      double bestGridObjectiveValue = std::numeric_limits<double>::infinity();

      if (arma::all((this->getUpperBounds() - this->getLowerBounds()) / arma::pow(numberOfSamples - 1, resolutionDepth + 1) < minimalSamplingDistances_)) {
        bestGridSoftConstraintsValue = gridSoftCostraintsValueThreshold;
        bestGridSoftConstraintsValue = gridObjectiveValueThreshold;

        unsigned int bestResolutionDepth = 0;
        for (std::size_t n = 0; n < samplesPerResolutions.size(); ++n) {
          for (const auto& sample : samplesPerResolutions.at(n)) {
            const double& candidateSoftConstraintsValue = sample.second.first;
            const double& candidateObjectiveValue = sample.second.second;

            if(candidateSoftConstraintsValue < bestGridSoftConstraintsValue || (candidateSoftConstraintsValue == bestGridSoftConstraintsValue && candidateObjectiveValue < bestGridObjectiveValue)) {
              bestGridParameter = sample.first;
              bestGridSoftConstraintsValue = candidateSoftConstraintsValue;
              bestGridObjectiveValue = candidateObjectiveValue;

              bestResolutionDepth = n;
            }
          }
        }

        resolutionDepth = bestResolutionDepth;
      } else {
        for(const auto& sample : samplesPerResolutions.at(resolutionDepth)) {
          const double& candidateSoftConstraintsValue = sample.second.first;
          const double& candidateObjectiveValue = sample.second.second;

          if(candidateSoftConstraintsValue < bestGridSoftConstraintsValue || (candidateSoftConstraintsValue == bestGridSoftConstraintsValue && candidateObjectiveValue < bestGridObjectiveValue)) {
            bestGridParameter = sample.first;
            bestGridSoftConstraintsValue = candidateSoftConstraintsValue;
            bestGridObjectiveValue = candidateObjectiveValue;
          }
        }

        ++resolutionDepth;
      }

      const arma::Col<double>& stepSize = (this->getUpperBounds() - this->getLowerBounds()) / arma::pow(numberOfSamples - 1, resolutionDepth);

      gridSoftCostraintsValueThreshold = bestGridSoftConstraintsValue;
      gridObjectiveValueThreshold = bestGridObjectiveValue;

      arma::Col<double> gridLowerBoundsCandidate = bestGridParameter - stepSize;
      arma::Col<double> gridUpperBoundsCandidate = bestGridParameter + stepSize;

      belowLowerBound = arma::find(gridLowerBoundsCandidate < gridLowerBounds);
      aboveUpperBound = arma::find(gridUpperBoundsCandidate > gridUpperBounds);

      gridLowerBoundsCandidate.elem(belowLowerBound) = gridLowerBounds.elem(belowLowerBound);
      gridUpperBoundsCandidate.elem(aboveUpperBound) = gridUpperBounds.elem(aboveUpperBound);

      gridLowerBounds = gridLowerBoundsCandidate;
      gridUpperBounds = gridUpperBoundsCandidate;

      if(samplesPerResolutions.find(resolutionDepth) == samplesPerResolutions.cend()) {
        samplesPerResolutions.insert({resolutionDepth, std::unordered_map<arma::Col<double>, std::pair<double, double>, Hash, IsEqual>()});
      }
    }
  }


  template <typename T>
  void MultiResolutionGridSearch<T>::setMinimalSamplingDistances(
      const arma::Col<double> minimalSamplingDistances) {
    if(minimalSamplingDistances.n_elem != this->numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the sampling distances (" + std::to_string(minimalSamplingDistances.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->numberOfDimensions_) + ").");
    }

    minimalSamplingDistances_ = minimalSamplingDistances;
  }

  template <typename T>
  void MultiResolutionGridSearch<T>::setMaximalSamplesPerResolution(
      const unsigned int maximalSamplesPerResolution) noexcept {
    maximalSamplesPerResolution_ = maximalSamplesPerResolution;
  }

  template <typename T>
  void MultiResolutionGridSearch<T>::setSamplingDistributionPerDimension(
      const arma::Col<double> samplingDistributionPerDimension) {
    if(samplingDistributionPerDimension.n_elem != this->numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the sampling distributions (" + std::to_string(samplingDistributionPerDimension.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->numberOfDimensions_) + ").");
    } else if(arma::sum(samplingDistributionPerDimension) != 1.0) {
      throw std::logic_error("The sum of all sampling distributions (" + std::to_string(arma::sum(samplingDistributionPerDimension)) + ") must be 1.");
    }

    samplingDistributionPerDimension_ = samplingDistributionPerDimension;
  }

  template <typename T>
  std::string MultiResolutionGridSearch<T>::toString() const noexcept {
    return "MultiResolutionGridSearch";
  }
}
