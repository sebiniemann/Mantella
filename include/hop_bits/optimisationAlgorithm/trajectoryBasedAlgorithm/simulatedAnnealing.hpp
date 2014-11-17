#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>

namespace hop {
  class SimulatedAnnealing : public TrajectoryBasedAlgorithm {
    public:
      explicit SimulatedAnnealing(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      SimulatedAnnealing(const SimulatedAnnealing&) = delete;
      SimulatedAnnealing& operator=(const SimulatedAnnealing&) = delete;

      void setMaximalStepSize(
          const arma::Col<double>& maximalStepSize);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> maximalStepSize_;

      arma::Col<double> state_;

      arma::Col<double> candidateParameter_;
      double candidateSoftConstraintValue_;
      double candidateObjectiveValue_;

      bool isAcceptableState();
      arma::Col<double> getVelocity();
      void optimiseImplementation() override;
  };
}
