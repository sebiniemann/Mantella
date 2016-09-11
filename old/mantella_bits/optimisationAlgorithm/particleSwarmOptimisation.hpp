#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class ParticleSwarmOptimisation : public OptimisationAlgorithm {
   public:
    explicit ParticleSwarmOptimisation();

    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setMaximalAcceleration(
        const double maximalAcceleration);
    double getMaximalAcceleration() const;

    void setMaximalLocalAttraction(
        const double maximalLocalAttraction);
    double getMaximalLocalAttraction() const;

    void setMaximalGlobalAttraction(
        const double maximalGlobalAttraction);
    double getMaximalGlobalAttraction() const;

   protected:
    double maximalAcceleration_;
    double maximalLocalAttraction_;
    double maximalGlobalAttraction_;

    arma::mat velocities_;

    arma::mat localBestSolutions_;
    arma::vec localBestObjectiveValues_;
  };
}
