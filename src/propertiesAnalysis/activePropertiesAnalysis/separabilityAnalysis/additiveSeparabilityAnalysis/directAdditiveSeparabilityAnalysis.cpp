#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis/directAdditiveSeparabilityAnalysis.hpp>

// Quick
#include <iostream>

// HOP
#include <hop_bits/optimisationAlgorithm/samplingBasedAlgorithm/gridSearch.hpp>
#include <hop_bits/helper/combinatorics.hpp>

namespace hop {
  void DirectAdditiveSeparabilityAnalysis::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept {
    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> partitionCandidates;
    for(unsigned int n = 1; n <= std::floor(optimisationProblem->getNumberOfDimensions() / 2); ++n) {
      for(const auto& combination : getCombinationsWithoutRepetition(optimisationProblem->getNumberOfDimensions(), n)) {
        arma::Col<arma::uword> complement(optimisationProblem->getNumberOfDimensions() - combination.n_elem);
        std::size_t k = 0;
        for(std::size_t l = 0; l < optimisationProblem->getNumberOfDimensions(); ++l) {
          if(!arma::any(combination == l)) {
            complement.at(k++) = l;
          }
        }
        partitionCandidates.push_back({combination, complement});
      }
    }

    std::vector<arma::Col<arma::uword>> partition;
    partition.push_back(arma::linspace<arma::Col<arma::uword>>(0, optimisationProblem->getNumberOfDimensions() - 1, optimisationProblem->getNumberOfDimensions()));
    for(const auto& partitionCandidate : partitionCandidates) {
      arma::Col<double> differences(maximalNumberOfIterations_);

      for(std::size_t n = 0; n < differences.n_elem; ++n) {
        arma::Col<double> firstPartA = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> firstPartB = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> secondPartA = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);
        arma::Col<double> secondPartB = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);

        arma::Col<double> candidateA(optimisationProblem->getNumberOfDimensions(), arma::fill::zeros);
        candidateA.elem(partitionCandidate.first) = firstPartA;
        candidateA.elem(partitionCandidate.second) = secondPartA;

        arma::Col<double> candidateB(optimisationProblem->getNumberOfDimensions(), arma::fill::zeros);
        candidateB.elem(partitionCandidate.first) = firstPartA;
        candidateB.elem(partitionCandidate.second) = secondPartB;

        arma::Col<double> candidateC(optimisationProblem->getNumberOfDimensions(), arma::fill::zeros);
        candidateC.elem(partitionCandidate.first) = firstPartB;
        candidateC.elem(partitionCandidate.second) = secondPartA;

        arma::Col<double> candidateD(optimisationProblem->getNumberOfDimensions(), arma::fill::zeros);
        candidateD.elem(partitionCandidate.first) = firstPartB;
        candidateD.elem(partitionCandidate.second) = secondPartB;

        differences.at(n) =
            optimisationProblem->getObjectiveValue(candidateA) +
            optimisationProblem->getObjectiveValue(candidateD) -
            optimisationProblem->getObjectiveValue(candidateB) -
            optimisationProblem->getObjectiveValue(candidateC);
      }
    }
  }
}
