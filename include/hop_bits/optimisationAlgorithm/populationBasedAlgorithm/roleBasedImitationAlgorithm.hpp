#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {
  class RoleBasedImitationAlgorithm : public PopulationBasedAlgorithm {
    public:
      explicit RoleBasedImitationAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem,
          const unsigned int& populationSize) noexcept;

      RoleBasedImitationAlgorithm(const RoleBasedImitationAlgorithm&) = delete;
      RoleBasedImitationAlgorithm& operator=(const RoleBasedImitationAlgorithm&) = delete;

      void setNeighbourhoodSize(
          const unsigned int& neighbourhoodSize) noexcept;
      void setStepSize(
          const double& stepSize) noexcept;
      void setMaximalNeighourhoodConvergence(
          const arma::Col<double>& maximalNeighourhoodConvergence) noexcept;

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> stepSize_;
      unsigned int neighbourhoodSize_;
      arma::Col<double> maximalNeighourhoodConvergence_;

      void optimiseImplementation() noexcept override;
  };
}
