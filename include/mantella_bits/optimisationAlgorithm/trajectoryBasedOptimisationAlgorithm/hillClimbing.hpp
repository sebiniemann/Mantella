#pragma once

// C++ standard library
#include <string>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

// TODO Add restarting
namespace mant {
  class HillClimbing : public TrajectoryBasedOptimisationAlgorithm {
   public:
    explicit HillClimbing(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setMinimalStepSize(
        const arma::Col<double>& minimalStepSize);

    void setMaximalStepSize(
        const arma::Col<double>& maximalStepSize);

    std::string toString() const override;

   protected:
    arma::Col<double> minimalStepSize_;
    arma::Col<double> maximalStepSize_;

    void optimiseImplementation() override;
  };
}
