#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class StepEllipsoidalFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit StepEllipsoidalFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec firstParameterConditioning_;
      const arma::vec secondParameterConditioning_;
      // Keeps randomly set data non-`const`, to be changed within tests.
      arma::mat rotationQ_;
    };
  }
}
