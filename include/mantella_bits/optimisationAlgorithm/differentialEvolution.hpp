#pragma once

// C++ standard library
#include <functional>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"

namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class DifferentialEvolution : public OptimisationAlgorithm {
   public:
    explicit DifferentialEvolution();

    void initialise(
        const arma::uword numberOfDimensions,
        const arma::Mat<double>& initialParameters) override;

    // Adds *optimise(OptimisationProblem& optimisationProblem, const arma::Mat<double>& initialParameters)*
    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setPopulationSize(
        const arma::uword populationSize);
    arma::uword getPopulationSize() const;
    void setAmplificationFactor(
        const double amplificationFactor);
    double getAmplificationFactor() const;
    void setCrossoverConstant(
        const double crossoverConstant);
    double getCrossoverConstant() const;
    void setMutationFunction(
        const std::function<double(arma::uword)> mutationFunction);
    std::function<double(arma::uword)> getMutationFunction() const;

    arma::uword populationSize_;
    arma::uword populationIndex_;
    double amplificationFactor_;
    double crossoverConstant_;
    std::function<double(arma::uword)> mutationFunction_;

    arma::Mat<double> population_;
    arma::Mat<double> localBestSolutions_;
    arma::Col<double> localBestObjectiveValues_;
  };
}
