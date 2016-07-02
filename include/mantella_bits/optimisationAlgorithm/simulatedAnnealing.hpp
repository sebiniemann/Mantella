#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class SimulatedAnnealing : public OptimisationAlgorithm {
   public:
    explicit SimulatedAnnealing();

    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setMinimalStepSize(
        const double minimalStepSize);
    double getMinimalStepSize() const;

    void setMaximalStepSize(
        const double maximalStepSize);
    double getMaximalStepSize() const;

    // Public accessible to be used in lambdas
    arma::vec state_;

   protected:
    double minimalStepSize_;
    double maximalStepSize_;
  };
}
