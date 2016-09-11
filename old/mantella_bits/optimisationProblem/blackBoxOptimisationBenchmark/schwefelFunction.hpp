#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class SchwefelFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit SchwefelFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec parameterConditioning_;
    };
  }
}
