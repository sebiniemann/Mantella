#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class RastriginFunctionRotated : public BlackBoxOptimisationBenchmark {
     public:
      explicit RastriginFunctionRotated(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec parameterConditioning_;
      // Keeps randomly set data non-`const`, to be set within tests and Mex files.
      arma::mat rotationQ_;
      arma::mat rotationR_;
    };
  }
}
