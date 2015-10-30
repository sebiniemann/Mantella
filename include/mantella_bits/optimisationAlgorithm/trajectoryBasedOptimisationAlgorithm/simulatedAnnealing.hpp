#pragma once

// C++ standard library
#include <string>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationAlgorithm/trajectoryBasedOptimisationAlgorithm.hpp>

// TODO Add restarting
namespace mant {
  class SimulatedAnnealing : public TrajectoryBasedOptimisationAlgorithm {
   public:
    explicit SimulatedAnnealing(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setMaximalStepSize(
        const arma::Col<double>& maximalStepSize);

    std::string toString() const override;

   protected:
    arma::Col<double> maximalStepSize_;

    virtual bool isAcceptableState(
        const double candidateObjectiveValue);

    void optimiseImplementation() override;
  };
}
