#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class HookeJeevesAlgorithm : public OptimisationAlgorithm {
   public:
    explicit HookeJeevesAlgorithm();

    void initialise(
        const arma::uword numberOfDimensions,
        const arma::Mat<double>& initialParameters) override;

    // Adds *optimise(OptimisationProblem& optimisationProblem, const arma::Mat<double>& initialParameters)*
    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setInitialStepSize(
        const double initialStepSize);
    double getInitialStepSize() const;
    void setStepSizeDecrease(
        const double stepSizeDecrease);
    double getStepSizeDecrease() const;

    // The following variables are only in public scope, to be used inside lambdas
    double initialStepSize_;
    double stepSizeDecrease_;

    double stepSize_;
  };
}
