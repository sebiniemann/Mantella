#pragma once

// C++ standard library
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationAlgorithm.hpp>

namespace mant {
  class PopulationBasedOptimisationAlgorithm : public OptimisationAlgorithm {
   public:
    explicit PopulationBasedOptimisationAlgorithm(
        const std::shared_ptr<OptimisationProblem> optimisationProblem,
        const arma::uword populationSize);

    void setInitialPopulation(
        const arma::Mat<double>& initialPopulation);

    arma::Mat<double> getRandomPopulation() const;

   protected:
    const arma::uword populationSize_;
    arma::Mat<double> initialPopulation_;
  };
}
