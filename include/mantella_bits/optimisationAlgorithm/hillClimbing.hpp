#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class HillClimbing : public OptimisationAlgorithm {
   public:
    explicit HillClimbing();

    void initialise(
        const arma::uword numberOfDimensions,
        const arma::Mat<double>& initialParameters) override;

    // Adds *optimise(OptimisationProblem& optimisationProblem, const arma::Mat<double>& initialParameters)*
    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setMinimalStepSize(
        const double minimalStepSize);
    double getMinimalStepSize() const;
    void setMaximalStepSize(
        const double maximalStepSize);
    double getMaximalStepSize() const;

   protected:
    double minimalStepSize_;
    double maximalStepSize_;
  };
}
