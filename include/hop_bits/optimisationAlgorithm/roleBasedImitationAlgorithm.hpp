#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class RoleBasedImitationAlgorithm : public OptimisationAlgorithm {
    public:
      explicit RoleBasedImitationAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize);

      RoleBasedImitationAlgorithm(const RoleBasedImitationAlgorithm&) = delete;
      RoleBasedImitationAlgorithm& operator=(const RoleBasedImitationAlgorithm&) = delete;

      void setNeighbourhoodSize(
          const unsigned int& neighbourhoodSize);
      void setStepSize(
          const double& stepSize);
      void setMaximalNeighourhoodConvergence(
          const arma::Col<double>& maximalNeighourhoodConvergence);

      std::string to_string() const override;

    protected:
      const unsigned int populationSize_;

      arma::Col<double> stepSize_;
      unsigned int neighbourhoodSize_;
      arma::Col<double> maximalNeighourhoodConvergence_;

      void optimiseImplementation() override;
  };
}
