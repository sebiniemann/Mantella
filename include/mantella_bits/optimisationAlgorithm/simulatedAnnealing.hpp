#pragma once

// C++ standard library
#include <functional>
#include <string>

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

    void initialise(
        const arma::uword numberOfDimensions,
        const arma::Mat<double>& initialParameters) override;

    // Adds *optimise(OptimisationProblem& optimisationProblem, const arma::Mat<double>& initialParameters)*
    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setIsAcceptableStateFunction(
        std::function<bool(const double objectiveValue_)> isAcceptableStateFunction,
        const std::string& isAcceptableStateFunctionName);
    void setIsAcceptableStateFunction(
        std::function<bool(const double objectiveValue_)> isAcceptableStateFunction);
    std::string getIsAcceptableStateFunctionName() const;

    void setMinimalStepSize(
        const double minimalStepSize);
    double getMinimalStepSize() const;
    void setMaximalStepSize(
        const double maximalStepSize);
    double getMaximalStepSize() const;

   protected:
    std::function<bool(const double objectiveValue_)> isAcceptableStateFunction_;
    std::string isAcceptableStateFunctionName_;

    double minimalStepSize_;
    double maximalStepSize_;

    arma::Col<double> state_;
  };
}
