#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class DiscusFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit DiscusFunction(
          const arma::uword numberOfDimensions);
    };
  }
}
