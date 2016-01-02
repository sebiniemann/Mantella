#pragma once

// C++ standard library
#include <unordered_set>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class HookeJeevesAlgorithm : public OptimisationAlgorithm {
   public:
    explicit HookeJeevesAlgorithm();

    void initialise(
        const arma::uword numberOfDimensions,
        const arma::Mat<double>& initialParameters) override;

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setInitialStepSize(
        const double initialStepSize);

    void setStepSizeDecrease(
        const double stepSizeDecrease);

   protected:
    double initialStepSize_;
    double stepSizeDecrease_;

    double stepSize_;

    std::unordered_set<arma::Col<double>, Hash, IsEqual> previousBestParameters_;
  };
}
