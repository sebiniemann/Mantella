#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class SphereFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit SphereFunction(
          const arma::uword numberOfDimensions);
    };
  }
}
