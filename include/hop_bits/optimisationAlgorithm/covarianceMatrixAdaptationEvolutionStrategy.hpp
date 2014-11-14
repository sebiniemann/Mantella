#pragma once

#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class CovarianceMatrixAdaptationEvolutionStrategy : public OptimisationAlgorithm {
    public:
      //TODO: which input parameters should be added to the constructor?
      explicit CovarianceMatrixAdaptationEvolutionStrategy(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

      CovarianceMatrixAdaptationEvolutionStrategy(const CovarianceMatrixAdaptationEvolutionStrategy&) = delete;
      CovarianceMatrixAdaptationEvolutionStrategy& operator=(const CovarianceMatrixAdaptationEvolutionStrategy&) = delete;

      std::string to_string() const override;

    protected:
      void optimiseImplementation() override;
  };
}
