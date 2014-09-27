#pragma once

#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class RoleBasedImitationAlgorithm : public OptimisationAlgorithm {
    public:
      explicit RoleBasedImitationAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      RoleBasedImitationAlgorithm(const RoleBasedImitationAlgorithm&) = delete;
      RoleBasedImitationAlgorithm& operator=(const RoleBasedImitationAlgorithm&) = delete;

      void setMaximalStepSize(const arma::Col<double>& maximalStepSize);

    protected:
      arma::Col<double> maximalStepSize_;

      void optimiseImplementation() override;
  };
}
