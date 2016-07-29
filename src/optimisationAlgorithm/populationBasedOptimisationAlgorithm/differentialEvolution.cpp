#include "mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm/differentialEvolution.hpp"

// C++ standard library
#include <functional>
#include <limits>
#include <string>
#include <utility>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  DifferentialEvolution::DifferentialEvolution()
      : PopulationBasedOptimisationAlgorithm() {
    setInitialisingFunctions(
        {{[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& initialParameters_) {
            population_ = initialParameters_;

            return initialParameters_;
          },
          "Population initialisation"},
         {[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& initialParameters_) {
            localBestObjectiveValues_.set_size(populationSize_);
            localBestObjectiveValues_.fill(std::numeric_limits<double>::infinity());

            return initialParameters_;
          },
          "Local best objective values initialisation"}});

    setNextParametersFunctions(
        {{[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_) {
            for (arma::uword n = 0; n < populationSize_; ++n) {
              if (objectiveValues_(n) < localBestObjectiveValues_(n)) {
                population_.col(n) = parameters_.col(n);
              }
            }

            arma::mat populationCandidates(arma::size(population_));
            for (arma::uword n = 0; n < populationSize_; ++n) {
              arma::uvec randomIndices = randomPermutationVector(populationSize_, 3);
              populationCandidates.col(n) = population_.col(randomIndices(0)) + getScalingFactor() * (population_.col(randomIndices(1)) - population_.col(randomIndices(2)));
            }

            return populationCandidates;
          },
          "Differential evolution"}});

    setScalingFactor(0.5);
  }

  void DifferentialEvolution::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, uniformRandomNumbers(optimisationProblem.numberOfDimensions_, populationSize_));
  }

  void DifferentialEvolution::setScalingFactor(
      const double scalingFactor) {
    scalingFactor_ = scalingFactor;
  }

  double DifferentialEvolution::getScalingFactor() const {
    return scalingFactor_;
  }
}
