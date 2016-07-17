#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class ParticleSwarmOptimisation : public PopulationBasedOptimisationAlgorithm {
   public:
    explicit ParticleSwarmOptimisation();

    using PopulationBasedOptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    arma::uword getNumberOfParticles() const;

    void setMaximalAcceleration(
        const double maximalAcceleration);
    double getMaximalAcceleration() const;

    void setMaximalLocalAttraction(
        const double maximalLocalAttraction);
    double getMaximalLocalAttraction() const;

    void setMaximalGlobalAttraction(
        const double maximalGlobalAttraction);
    double getMaximalGlobalAttraction() const;

    // Public accessible to be used in lambdas
    arma::mat velocities_;

    arma::mat localBestSolutions_;
    arma::vec localBestObjectiveValues_;

   protected:
    arma::uword numberOfParticles_;

    double maximalAcceleration_;
    double maximalLocalAttraction_;
    double maximalGlobalAttraction_;
  };
}
