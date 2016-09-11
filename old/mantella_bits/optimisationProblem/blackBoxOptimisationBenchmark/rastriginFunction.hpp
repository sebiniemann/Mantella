#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class RastriginFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit RastriginFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec parameterConditioning_;
    };
  }
}
