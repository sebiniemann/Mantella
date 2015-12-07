#pragma once

// C++ standard library
#include <string>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class SimulatedAnnealing : public OptimisationAlgorithm {
   public:
    explicit SimulatedAnnealing();

    void optimise(
        const std::shared_ptr<OptimisationProblem> optimisationProblem,
        const arma::Mat<double>& initialParameters) override;

    void optimise(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setIsAcceptableStateFunction(
        std::function<bool(const double objectiveValue)> isAcceptableStateFunction);
        
    void setMinimalStepSize(
        const double minimalStepSize);

    void setMaximalStepSize(
        const double maximalStepSize);

   protected:
    std::function<bool(const double objectiveValue)> isAcceptableStateFunction_;
    
    double minimalStepSize_;
    double maximalStepSize_;
    
    arma::Col<double> state_;
  };
}
