#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>

namespace mant {
  class HillClimbing : public TrajectoryBasedAlgorithm<double> {
    public:
      explicit HillClimbing(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept;

      HillClimbing(const HillClimbing&) = delete;
      HillClimbing& operator=(const HillClimbing&) = delete;

      void setMaximalStepSize(
          const arma::Col<double>& maximalStepSize);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> maximalStepSize_;

      virtual arma::Col<double> getVelocity() noexcept;

      void optimiseImplementation() noexcept override;
  };
}
