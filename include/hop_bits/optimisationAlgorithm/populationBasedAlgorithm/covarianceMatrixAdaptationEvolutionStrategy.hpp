#pragma once

#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {
  class CovarianceMatrixAdaptationEvolutionStrategy : public PopulationBasedAlgorithm {
    public:
      //TODO: which input parameters should be added to the constructor?
      explicit CovarianceMatrixAdaptationEvolutionStrategy(
        const std::shared_ptr<OptimisationProblem> optimisationProblem,const unsigned int& populationSize);

      CovarianceMatrixAdaptationEvolutionStrategy(const CovarianceMatrixAdaptationEvolutionStrategy&) = delete;
      CovarianceMatrixAdaptationEvolutionStrategy& operator=(const CovarianceMatrixAdaptationEvolutionStrategy&) = delete;

      std::string to_string() const noexcept override;

    protected:
      void optimiseImplementation() override;
  };
}
