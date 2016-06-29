#pragma once

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"

namespace mant {
  class PopulationBasedOptimisationAlgorithm : public OptimisationAlgorithm {
   public:
    explicit PopulationBasedOptimisationAlgorithm();

    void optimise(
        OptimisationProblem& optimisationProblem,
        const arma::mat& initialParameters);

    void setPopulationSize(
        const arma::uword populationSize);

    arma::uword getPopulationSize() const;

   protected:
    arma::uword populationSize_;
  };
}