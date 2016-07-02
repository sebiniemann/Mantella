#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class SharpRidgeFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit SharpRidgeFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec parameterConditioning_;
      // Keeps randomly set data non-`const`, to be changed within tests.
      arma::mat rotationQ_;
    };
  }
}
