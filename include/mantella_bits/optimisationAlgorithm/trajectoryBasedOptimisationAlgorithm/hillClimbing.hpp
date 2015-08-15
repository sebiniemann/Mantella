#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm.hpp>

// TODO Add restarting
namespace mant {
  class HillClimbing : public TrajectoryBasedOptimisationAlgorithm {
    public:
      explicit HillClimbing(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      void setMaximalStepSize(
          const double maximalStepSize);

      std::string toString() const override;

    protected:
      double maximalStepSize_;

      void optimiseImplementation() override;
  };
}
