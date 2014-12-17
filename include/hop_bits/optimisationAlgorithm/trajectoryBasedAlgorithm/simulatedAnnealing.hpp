#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>

namespace hop {
  class SimulatedAnnealing : public TrajectoryBasedAlgorithm<double> {
    public:
      explicit SimulatedAnnealing(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept;

      SimulatedAnnealing(const SimulatedAnnealing&) = delete;
      SimulatedAnnealing& operator=(const SimulatedAnnealing&) = delete;

      void setMaximalStepSize(
          const arma::Col<double>& maximalStepSize);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> maximalStepSize_;

      virtual bool isAcceptableState(
          const double& candidateObjectiveValue) noexcept;
      virtual arma::Col<double> getVelocity() noexcept;

      void optimiseImplementation() noexcept override;
  };
}
