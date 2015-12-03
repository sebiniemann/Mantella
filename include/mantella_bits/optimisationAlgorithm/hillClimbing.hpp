#pragma once

// C++ standard library
#include <string>
#include <memory>

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

    void optimise(
        const std::shared_ptr<OptimisationProblem> optimisationProblem,
        const arma::Mat<double>& initialParameters) override;

    void optimise(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setMinimalStepSize(
        const double minimalStepSize);

    void setMaximalStepSize(
        const double maximalStepSize);

   protected:
    double minimalStepSize_;
    double maximalStepSize_;
  };
}
