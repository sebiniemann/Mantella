#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class DifferentPowersFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit DifferentPowersFunction(
          const arma::uword numberOfDimensions);
    };
  }
}
