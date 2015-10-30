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
  class HookeJeevesAlgorithm : public TrajectoryBasedOptimisationAlgorithm {
   public:
    explicit HookeJeevesAlgorithm(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setInitialStepSize(
        const arma::Col<double>& initialStepSize);

    void setStepSizeDecrease(
        const arma::Col<double>& stepSizeDecrease);

    std::string toString() const override;

   protected:
    arma::Col<double> initialStepSize_;
    arma::Col<double> stepSizeDecrease_;

    void optimiseImplementation() override;
  };
}
