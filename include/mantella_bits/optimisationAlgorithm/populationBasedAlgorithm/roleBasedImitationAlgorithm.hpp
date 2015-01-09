#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace mant {
  class RoleBasedImitationAlgorithm : public PopulationBasedAlgorithm<double> {
    public:
      explicit RoleBasedImitationAlgorithm(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
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
