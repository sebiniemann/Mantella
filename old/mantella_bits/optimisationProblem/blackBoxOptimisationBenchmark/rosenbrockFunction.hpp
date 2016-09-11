#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class RosenbrockFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit RosenbrockFunction(
          const arma::uword numberOfDimensions);

     protected:
      const double max_;
    };
  }
}
