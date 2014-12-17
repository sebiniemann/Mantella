#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/multiResolutionGridSearch.hpp>

// C++ Standard Library
#include <vector>
#include <map>
#include <iostream>

// HOP
#include <hop_bits/helper/unordered_map.hpp>

namespace hop {
  MultiResolutionGridSearch::MultiResolutionGridSearch(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept
    : SamplingBasedAlgorithm<double>(optimisationProblem) {
    setMinimalSamplingDistances(arma::ones(optimisationProblem_->getNumberOfDimensions()) * 1e-3);
    setSamplingDistributionPerDimension(arma::ones(optimisationProblem_->getNumberOfDimensions()) / static_cast<double>(optimisationProblem_->getNumberOfDimensions()));
    setMaximalSamplesPerResolution(11);
  }

  void MultiResolutionGridSearch::optimiseImplementation() noexcept {
    unsigned int resolutionDepth = 0;

    std::map<unsigned int, std::unordered_map<arma::Col<double>, std::pair<double, double>, Hasher, KeyEqual>> samplesPerResolutions;
    samplesPerResolutions.insert({resolutionDepth, {}});

    double gridSoftCostraintsValueThreshold = std::numeric_limits<double>::infinity();
    double gridObjectiveValueThreshold = std::numeric_limits<double>::infinity();

    arma::Col<double> gridLowerBounds = optimisationProblem_->getLowerBounds();
    arma::Col<double> gridUpperBounds = optimisationProblem_->getUpperBounds();

    arma::Col<arma::uword> belowLowerBound(optimisationProblem_->getNumberOfDimensions(), arma::fill::zeros);
    arma::Col<arma::uword> aboveUpperBound(optimisationProblem_->getNumberOfDimensions(), arma::fill::zeros);
    while(!isFinished() & !isTerminated()) {
      const arma::Col<double>& scaledSamplingFactors = samplingDistributionPerDimension_.at(0) / samplingDistributionPerDimension_;
      const arma::Col<arma::uword>& numberOfSamples = arma::conv_to<arma::Col<arma::uword>>::from(scaledSamplingFactors * std::pow(std::pow(maximalSamplesPerResolution_ / std::pow(2, arma::sum(belowLowerBound) + arma::sum(aboveUpperBound)), optimisationProblem_->getNumberOfDimensions()) / arma::prod(scaledSamplingFactors), 1.0 / static_cast<double>(optimisationProblem_->getNumberOfDimensions())));

      std::vector<arma::Col<double>> sampleParameters_;
      for (std::size_t n = 0; n < optimisationProblem_->getNumberOfDimensions(); ++n) {
        sampleParameters_.push_back(arma::linspace(gridLowerBounds.at(n), gridUpperBounds.at(n), numberOfSamples.at(n)));
      }

      arma::Col<arma::uword> sampleIndicies_ = arma::zeros<arma::Col<arma::uword>>(sampleParameters_.size());
      arma::Col<double> candidateParameter(optimisationProblem_->getNumberOfDimensions());

      const unsigned int& overallNumberOfSamples = arma::prod(numberOfSamples);
      for(unsigned int n = 0; n < overallNumberOfSamples; ++n) {
        ++numberOfIterations_;

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

        const double& candidateSoftConstraintsValue = optimisationProblem_->getSoftConstraintsValue(candidateParameter);
        const double& candidateObjectiveValue = optimisationProblem_->getObjectiveValue(candidateParameter);

        samplesPerResolutions.at(resolutionDepth).insert({candidateParameter, {candidateSoftConstraintsValue, candidateObjectiveValue}});

        if(candidateSoftConstraintsValue < bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == bestSoftConstraintsValue_ && candidateObjectiveValue < bestObjectiveValue_)) {
          bestParameter_ = candidateParameter;
          bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
          bestObjectiveValue_ = candidateObjectiveValue;
        }

        if(isFinished() || isTerminated()) {
          break;
        }
      }

      arma::Col<double> bestGridParameter;
      double bestGridSoftConstraintsValue = std::numeric_limits<double>::infinity();
      double bestGridObjectiveValue = std::numeric_limits<double>::infinity();

      if (arma::all((optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) / arma::pow(numberOfSamples - 1, resolutionDepth + 1) < minimalSamplingDistances_)) {
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

      const arma::Col<double>& stepSize = (optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds()) / arma::pow(numberOfSamples - 1, resolutionDepth);

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
        samplesPerResolutions.insert({resolutionDepth, {}});
      }
    }
  }


  void MultiResolutionGridSearch::setMinimalSamplingDistances(
      const arma::Col<double>& minimalSamplingDistances) {
    if(minimalSamplingDistances.n_elem != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the sampling distances (" + std::to_string(minimalSamplingDistances.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    }

    minimalSamplingDistances_ = minimalSamplingDistances;
  }

  void MultiResolutionGridSearch::setMaximalSamplesPerResolution(
      const unsigned int& maximalSamplesPerResolution) noexcept {
    maximalSamplesPerResolution_ = maximalSamplesPerResolution;
  }

  void MultiResolutionGridSearch::setSamplingDistributionPerDimension(
      const arma::Col<double>& samplingDistributionPerDimension) {
    if(samplingDistributionPerDimension.n_elem != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the sampling distributions (" + std::to_string(samplingDistributionPerDimension.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if(arma::sum(samplingDistributionPerDimension) != 1.0) {
      throw std::logic_error("The sum of all sampling distributions (" + std::to_string(arma::sum(samplingDistributionPerDimension)) + ") must be 1.");
    }

    samplingDistributionPerDimension_ = samplingDistributionPerDimension;
  }

  std::string MultiResolutionGridSearch::to_string() const noexcept {
    return "MultiResolutionGridSearch";
  }
}
