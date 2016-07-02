#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class SchaffersF7FunctionIllConditioned : public BlackBoxOptimisationBenchmark {
     public:
      explicit SchaffersF7FunctionIllConditioned(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec parameterConditioning_;
      // Keeps randomly set data non-`const`, to be changed within tests.
      arma::mat rotationQ_;
    };
  }
}
