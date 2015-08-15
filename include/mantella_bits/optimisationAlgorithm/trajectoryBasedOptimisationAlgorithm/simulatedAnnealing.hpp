#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm.hpp>

// TODO Add restarting
namespace mant {
  class SimulatedAnnealing : public TrajectoryBasedOptimisationAlgorithm {
    public:
      explicit SimulatedAnnealing(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      void setMaximalStepSize(
          const double maximalStepSize);

      std::string toString() const override;

    protected:
      double maximalStepSize_;

      virtual bool isAcceptableState(
          const double candidateObjectiveValue);

      void optimiseImplementation() override;
  };
}
