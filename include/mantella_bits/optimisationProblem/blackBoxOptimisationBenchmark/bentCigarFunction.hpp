#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class BentCigarFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit BentCigarFunction(
          const arma::uword numberOfDimensions);

     protected:
      // Keeps randomly set data non-`const`, to be changed within tests.
      arma::mat rotationQ_;
    };
  }
}
