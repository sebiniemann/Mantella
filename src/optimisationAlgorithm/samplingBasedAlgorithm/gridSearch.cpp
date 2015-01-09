#include <mantella_bits/optimisationAlgorithm/samplingBasedAlgorithm/gridSearch.hpp>

// C++ Standard Library
#include <vector>

namespace mant {
  GridSearch::GridSearch(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept
    : SamplingBasedAlgorithm(optimisationProblem) {
    setSamplingFactors(arma::ones(optimisationProblem_->getNumberOfDimensions()) / static_cast<double>(optimisationProblem_->getNumberOfDimensions()));
  }

  void GridSearch::optimiseImplementation() noexcept {
    const arma::Col<double>& scaledSamplingFactors = samplingFactors_.at(0) / samplingFactors_;
    const arma::Col<unsigned int>& numberOfSamples_ = arma::conv_to<arma::Col<unsigned int>>::from(scaledSamplingFactors * std::pow(maximalNumberOfIterations_ / arma::prod(scaledSamplingFactors), 1.0 / static_cast<double>(optimisationProblem_->getNumberOfDimensions())));

    std::vector<arma::Col<double>> sampleParameters_;
    for (std::size_t n = 0; n < optimisationProblem_->getNumberOfDimensions(); ++n) {
      sampleParameters_.push_back(arma::linspace(optimisationProblem_->getLowerBounds().at(n), optimisationProblem_->getUpperBounds().at(n), numberOfSamples_.at(n)));
    }

    arma::Col<unsigned int> sampleIndicies_ = arma::zeros<arma::Col<unsigned int>>(sampleParameters_.size());
    arma::Col<double> candidateParameter(optimisationProblem_->getNumberOfDimensions());

    const unsigned int& overallNumberOfSamples = arma::sum(numberOfSamples_);
    for(unsigned int n = 0; n < overallNumberOfSamples; ++n) {
      ++numberOfIterations_;

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

      const double& candidateSoftConstraintsValue = optimisationProblem_->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = optimisationProblem_->getObjectiveValue(candidateParameter);

      if(candidateSoftConstraintsValue < bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == bestSoftConstraintsValue_ && candidateObjectiveValue < bestObjectiveValue_)) {
        bestParameter_ = candidateParameter;
        bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
        bestObjectiveValue_ = candidateObjectiveValue;
      }

      if (isFinished() || isTerminated()) {
        break;
      }
    }
  }

  void GridSearch::setSamplingFactors(
      const arma::Col<double>& samplingFactors) {
    if(samplingFactors.n_elem != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the sampling factors (" + std::to_string(samplingFactors.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if(arma::sum(samplingFactors) != 1.0) {
      throw std::logic_error("The sum of all sampling factors (" + std::to_string(arma::sum(samplingFactors)) + ") must be 1.");
    }

    samplingFactors_ = samplingFactors;
  }

  std::string GridSearch::to_string() const noexcept {
    return "GridSearch";
  }
}
