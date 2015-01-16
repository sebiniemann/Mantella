#pragma once

// C++ Standard Library
#include <vector>
#include <map>

// Mantella
#include <mantella_bits/optimisationAlgorithm/samplingBasedAlgorithm.hpp>
#include <mantella_bits/helper/unordered_container.hpp>

namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class MultiResolutionGridSearch : public SamplingBasedAlgorithm<ParameterType, DistanceFunction> {
    public:
      explicit MultiResolutionGridSearch(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      MultiResolutionGridSearch(const MultiResolutionGridSearch&) = delete;
      MultiResolutionGridSearch& operator=(const MultiResolutionGridSearch&) = delete;

      void setMinimalSamplingDistances(
          const arma::Col<double>& minimalSamplingDistances);

      void setMaximalSamplesPerResolution(
          const unsigned int& maximalSamplesPerResolution) noexcept;

      void setSamplingDistributionPerDimension(
          const arma::Col<double>& samplingDistributionPerDimension);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<ParameterType> minimalSamplingDistances_;
      unsigned int maximalSamplesPerResolution_;
      arma::Col<double> samplingDistributionPerDimension_;

      void optimiseImplementation() noexcept override;
  };

  template <typename ParameterType, class DistanceFunction>
  MultiResolutionGridSearch<ParameterType, DistanceFunction>::MultiResolutionGridSearch(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : SamplingBasedAlgorithm<ParameterType, DistanceFunction>(optimisationProblem) {
    setMinimalSamplingDistances(arma::ones(this->optimisationProblem_->getNumberOfDimensions()) * 1e-3);
    setSamplingDistributionPerDimension(arma::ones(this->optimisationProblem_->getNumberOfDimensions()) / static_cast<double>(this->optimisationProblem_->getNumberOfDimensions()));
    setMaximalSamplesPerResolution(11);
  }

  template <typename ParameterType, class DistanceFunction>
  void MultiResolutionGridSearch<ParameterType, DistanceFunction>::optimiseImplementation() noexcept {
    unsigned int resolutionDepth = 0;

    std::map<unsigned int, std::unordered_map<arma::Col<double>, std::pair<double, double>, Hash<arma::Col<double>>, IsKeyEqual<arma::Col<double>>>> samplesPerResolutions;
    samplesPerResolutions.insert({resolutionDepth, std::unordered_map<arma::Col<double>, std::pair<double, double>, Hash<arma::Col<double>>, IsKeyEqual<arma::Col<double>>>()});

    double gridSoftCostraintsValueThreshold = std::numeric_limits<double>::infinity();
    double gridObjectiveValueThreshold = std::numeric_limits<double>::infinity();

    arma::Col<double> gridLowerBounds = this->optimisationProblem_->getLowerBounds();
    arma::Col<double> gridUpperBounds = this->optimisationProblem_->getUpperBounds();

    arma::Col<unsigned int> belowLowerBound(this->optimisationProblem_->getNumberOfDimensions(), arma::fill::zeros);
    arma::Col<unsigned int> aboveUpperBound(this->optimisationProblem_->getNumberOfDimensions(), arma::fill::zeros);
    while(!this->isFinished() & !this->isTerminated()) {
      const arma::Col<double>& scaledSamplingFactors = samplingDistributionPerDimension_.at(0) / samplingDistributionPerDimension_;
      const arma::Col<unsigned int>& numberOfSamples = arma::conv_to<arma::Col<unsigned int>>::from(scaledSamplingFactors * std::pow(std::pow(maximalSamplesPerResolution_ / std::pow(2, arma::sum(belowLowerBound) + arma::sum(aboveUpperBound)), this->optimisationProblem_->getNumberOfDimensions()) / arma::prod(scaledSamplingFactors), 1.0 / static_cast<double>(this->optimisationProblem_->getNumberOfDimensions())));

      std::vector<arma::Col<double>> sampleParameters_;
      for (std::size_t n = 0; n < this->optimisationProblem_->getNumberOfDimensions(); ++n) {
        sampleParameters_.push_back(arma::linspace(gridLowerBounds.at(n), gridUpperBounds.at(n), numberOfSamples.at(n)));
      }

      arma::Col<unsigned int> sampleIndicies_ = arma::zeros<arma::Col<unsigned int>>(sampleParameters_.size());
      arma::Col<double> candidateParameter(this->optimisationProblem_->getNumberOfDimensions());

      const unsigned int& overallNumberOfSamples = arma::prod(numberOfSamples);
      for(unsigned int n = 0; n < overallNumberOfSamples; ++n) {
        ++this->numberOfIterations_;

        for(std::size_t k = 0; k < sampleIndicies_.n_elem; ++k) {
          candidateParameter.at(k) = sampleParameters_.at(k).at(sampleIndicies_.at(k));
        }

        ++sampleIndicies_.at(0);
        for(std::size_t k = 0; k < sampleIndicies_.n_elem - 1; ++k) {
          if(sampleIndicies_.at(k) >= numberOfSamples.at(k)) {
            sampleIndicies_.at(k) = 0;
             ++sampleIndicies_.at(k + 1);
          }
        }

        const double& candidateSoftConstraintsValue = this->optimisationProblem_->getSoftConstraintsValue(candidateParameter);
        const double& candidateObjectiveValue = this->optimisationProblem_->getObjectiveValue(candidateParameter);

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

      if (arma::all((this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds()) / arma::pow(numberOfSamples - 1, resolutionDepth + 1) < minimalSamplingDistances_)) {
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

      const arma::Col<double>& stepSize = (this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds()) / arma::pow(numberOfSamples - 1, resolutionDepth);

      gridSoftCostraintsValueThreshold = bestGridSoftConstraintsValue;
      gridObjectiveValueThreshold = bestGridObjectiveValue;

      std::cout << bestGridParameter << std::endl;
      std::cout << stepSize << std::endl;
      arma::Col<double> gridLowerBoundsCandidate = bestGridParameter - stepSize;
      arma::Col<double> gridUpperBoundsCandidate = bestGridParameter + stepSize;

      belowLowerBound = arma::find(gridLowerBoundsCandidate < gridLowerBounds);
      aboveUpperBound = arma::find(gridUpperBoundsCandidate > gridUpperBounds);

      gridLowerBoundsCandidate.elem(belowLowerBound) = gridLowerBounds.elem(belowLowerBound);
      gridUpperBoundsCandidate.elem(aboveUpperBound) = gridUpperBounds.elem(aboveUpperBound);

      gridLowerBounds = gridLowerBoundsCandidate;
      gridUpperBounds = gridUpperBoundsCandidate;

      if(samplesPerResolutions.find(resolutionDepth) == samplesPerResolutions.end()) {
        samplesPerResolutions.insert({resolutionDepth, std::unordered_map<arma::Col<double>, std::pair<double, double>, Hash<arma::Col<double>>, IsKeyEqual<arma::Col<double>>>()});
      }
    }
  }


  template <typename ParameterType, class DistanceFunction>
  void MultiResolutionGridSearch<ParameterType, DistanceFunction>::setMinimalSamplingDistances(
      const arma::Col<double>& minimalSamplingDistances) {
    if(minimalSamplingDistances.n_elem != this->optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the sampling distances (" + std::to_string(minimalSamplingDistances.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->optimisationProblem_->getNumberOfDimensions()) + ").");
    }

    minimalSamplingDistances_ = minimalSamplingDistances;
  }

  template <typename ParameterType, class DistanceFunction>
  void MultiResolutionGridSearch<ParameterType, DistanceFunction>::setMaximalSamplesPerResolution(
      const unsigned int& maximalSamplesPerResolution) noexcept {
    maximalSamplesPerResolution_ = maximalSamplesPerResolution;
  }

  template <typename ParameterType, class DistanceFunction>
  void MultiResolutionGridSearch<ParameterType, DistanceFunction>::setSamplingDistributionPerDimension(
      const arma::Col<double>& samplingDistributionPerDimension) {
    if(samplingDistributionPerDimension.n_elem != this->optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the sampling distributions (" + std::to_string(samplingDistributionPerDimension.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if(arma::sum(samplingDistributionPerDimension) != 1.0) {
      throw std::logic_error("The sum of all sampling distributions (" + std::to_string(arma::sum(samplingDistributionPerDimension)) + ") must be 1.");
    }

    samplingDistributionPerDimension_ = samplingDistributionPerDimension;
  }

  template <typename ParameterType, class DistanceFunction>
  std::string MultiResolutionGridSearch<ParameterType, DistanceFunction>::to_string() const noexcept {
    return "MultiResolutionGridSearch";
  }
}
