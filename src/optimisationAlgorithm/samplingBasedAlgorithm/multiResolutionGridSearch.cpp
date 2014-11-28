#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/multiResolutionGridSearch.hpp>

// C++ Standard Library
#include <vector>
#include <map>

// HOP
#include <hop_bits/helper/cache.hpp>

namespace hop {
  MultiResolutionGridSearch::MultiResolutionGridSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept
    : SamplingBasedAlgorithm(optimisationProblem) {
    setSamplingFactors(arma::ones(optimisationProblem_->getNumberOfDimensions()) / static_cast<double>(optimisationProblem_->getNumberOfDimensions()));
    setAverageSamplesPerDimension(11);
  }

  void MultiResolutionGridSearch::optimiseImplementation() noexcept {
    unsigned int resolutionDepth = 0;

    std::map<unsigned int, std::unordered_map<arma::Col<double>, std::pair<double, double>, CacheHasher, CacheKeyEqual>> samplesPerResolution;
    samplesPerResolution.insert({resolutionDepth, {}});

    std::map<unsigned int, double> bestSoftCostraintsPerResolution;
    bestSoftCostraintsPerResolution.insert({resolutionDepth, std::numeric_limits<double>::infinity()});

    std::map<unsigned int, double> bestObjectiveValuePerResolution;
    bestObjectiveValuePerResolution.insert({resolutionDepth, std::numeric_limits<double>::infinity()});

    std::map<unsigned int, arma::Col<double>> lowerGridBoundsPerResolution;
    lowerGridBoundsPerResolution.insert({resolutionDepth, optimisationProblem_->getLowerBounds()});

    std::map<unsigned int, arma::Col<double>> upperGridBoundsPerResolution;
    upperGridBoundsPerResolution.insert({resolutionDepth, optimisationProblem_->getUpperBounds()});

    while(!isFinished() & !isTerminated()) {
      const arma::Col<double>& scaledSamplingFactors = samplingFactors_.at(0) / samplingFactors_;
      const arma::Col<arma::uword>& numberOfSamples = arma::conv_to<arma::Col<arma::uword>>::from(scaledSamplingFactors * std::pow(std::pow(averageSamplesPerDimension_, optimisationProblem_->getNumberOfDimensions()) / arma::prod(scaledSamplingFactors), 1.0 / static_cast<double>(optimisationProblem_->getNumberOfDimensions())));

      std::vector<arma::Col<double>> sampleParameters_;
      for (std::size_t n = 0; n < optimisationProblem_->getNumberOfDimensions(); ++n) {
        sampleParameters_.push_back(arma::linspace(lowerGridBoundsPerResolution.at(resolutionDepth).at(n), upperGridBoundsPerResolution.at(resolutionDepth).at(n), numberOfSamples.at(n)));
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

        samplesPerResolution.at(resolutionDepth).insert({candidateParameter, {candidateSoftConstraintsValue, candidateObjectiveValue}});

        if(candidateSoftConstraintsValue < bestSoftConstraintsValue_ || candidateSoftConstraintsValue == bestSoftConstraintsValue_ && candidateObjectiveValue < bestObjectiveValue_) {
          bestParameter_ = candidateParameter;
          bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
          bestObjectiveValue_ = candidateObjectiveValue;
        }

        if(isFinished() || isTerminated()) {
          break;
        }

      }

      bool foundSomething = false;
      arma::Col<double> refinedGridParameter;
      double refinedGridSoftConstraintsValue = bestSoftCostraintsPerResolution.at(resolutionDepth);
      double refinedGridObjectiveValue = bestObjectiveValuePerResolution.at(resolutionDepth);

      for(const auto& sample : samplesPerResolution.at(resolutionDepth)) {
        const double& candidateSoftConstraintsValue = sample.second.first;
        const double& candidateObjectiveValue = sample.second.second;

        if(candidateSoftConstraintsValue < refinedGridSoftConstraintsValue || candidateSoftConstraintsValue == refinedGridSoftConstraintsValue && candidateObjectiveValue < refinedGridObjectiveValue) {
          refinedGridParameter = sample.first;
          refinedGridSoftConstraintsValue = candidateSoftConstraintsValue;
          refinedGridObjectiveValue = candidateObjectiveValue;

          foundSomething = true;
        }
      }

      const arma::Col<double>& stepSize = (upperGridBoundsPerResolution.at(resolutionDepth) - lowerGridBoundsPerResolution.at(resolutionDepth)) / (numberOfSamples - 1);
      arma::Col<double> lowerGridBoundsCandidate = refinedGridParameter - stepSize;
      arma::Col<double> upperGridBoundsCandidate = refinedGridParameter + stepSize;

      const arma::Col<arma::uword>& belowLowerBound = arma::find(lowerGridBoundsCandidate < lowerGridBoundsPerResolution.at(resolutionDepth));
      const arma::Col<arma::uword>& aboveUpperBound = arma::find(upperGridBoundsCandidate > upperGridBoundsPerResolution.at(resolutionDepth));

      lowerGridBoundsCandidate.elem(belowLowerBound) = lowerGridBoundsPerResolution.at(resolutionDepth).elem(belowLowerBound);
      upperGridBoundsCandidate.elem(aboveUpperBound) = upperGridBoundsPerResolution.at(resolutionDepth).elem(aboveUpperBound);

      ++resolutionDepth;
      lowerGridBoundsPerResolution.insert({resolutionDepth, lowerGridBoundsCandidate});
      upperGridBoundsPerResolution.insert({resolutionDepth, upperGridBoundsCandidate});

      bestSoftCostraintsPerResolution.insert({resolutionDepth, refinedGridSoftConstraintsValue});
      bestObjectiveValuePerResolution.insert({resolutionDepth, refinedGridObjectiveValue});

      if(samplesPerResolution.find(resolutionDepth) == samplesPerResolution.end()) {
        samplesPerResolution.insert({resolutionDepth, {}});
      }
    }
  }

  void MultiResolutionGridSearch::setAverageSamplesPerDimension(
      const unsigned int& averageSamplesPerDimension) noexcept {
    averageSamplesPerDimension_ = averageSamplesPerDimension;
  }

  void MultiResolutionGridSearch::setSamplingFactors(
      const arma::Col<double>& samplingFactors) {
    if(samplingFactors.n_elem != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the sampling factors (" + std::to_string(samplingFactors.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if(arma::sum(samplingFactors) != 1.0) {
      throw std::logic_error("The sum of all sampling factors (" + std::to_string(arma::sum(samplingFactors)) + ") must be 1.");
    }

    samplingFactors_ = samplingFactors;
  }

  std::string MultiResolutionGridSearch::to_string() const noexcept {
    return "MultiResolutionGridSearch";
  }
}
