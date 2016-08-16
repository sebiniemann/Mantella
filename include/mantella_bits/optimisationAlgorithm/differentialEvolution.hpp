#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class DifferentialEvolution : public OptimisationAlgorithm {
   public:
    explicit DifferentialEvolution();

    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setScalingFactor(
        const double scalingFactor);
    double getScalingFactor() const;

   protected:
    double scalingFactor_;

    arma::mat population_;
    arma::vec localBestObjectiveValues_;
  };
}
