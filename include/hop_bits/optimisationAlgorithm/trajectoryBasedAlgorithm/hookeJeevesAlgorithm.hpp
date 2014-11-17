#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>

namespace hop {
  class HookeJeevesAlgorithm : public TrajectoryBasedAlgorithm {
    public:
      explicit HookeJeevesAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      HookeJeevesAlgorithm(const HookeJeevesAlgorithm&) = delete;
      HookeJeevesAlgorithm& operator=(const HookeJeevesAlgorithm&) = delete;

      void setInitialStepSize(
          const arma::Col<double>& initialStepSize);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> initialStepSize_;

      arma::Col<double> stepSize_;
      bool reduceStepSize_;

      arma::Col<double> candidateParameter_;
      double candidateSoftConstraintValue_;
      double candidateObjectiveValue_;

      arma::Col<double> getReducedStepSize();
      void optimiseImplementation() override;
  };
}
