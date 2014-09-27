#pragma once

#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class HookeJeevesAlgorithm : public OptimisationAlgorithm {
    public:
      explicit HookeJeevesAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      HookeJeevesAlgorithm(const HookeJeevesAlgorithm&) = delete;
      HookeJeevesAlgorithm& operator=(const HookeJeevesAlgorithm&) = delete;

    protected:
      arma::Col<double> stepSize_;
      bool reduceStepSize_;

      void optimiseImplementation() override;
  };
}
