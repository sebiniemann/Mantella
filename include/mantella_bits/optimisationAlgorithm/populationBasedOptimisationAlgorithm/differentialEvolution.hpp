#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class DifferentialEvolution : public PopulationBasedOptimisationAlgorithm {
   public:
    explicit DifferentialEvolution();

    using PopulationBasedOptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setScalingFactor(
        const double scalingFactor);
    double getScalingFactor() const;

    // Public accessible to be used in lambdas
    arma::mat population_;
    arma::vec localBestObjectiveValues_;

   protected:
    double scalingFactor_;
  };
}
