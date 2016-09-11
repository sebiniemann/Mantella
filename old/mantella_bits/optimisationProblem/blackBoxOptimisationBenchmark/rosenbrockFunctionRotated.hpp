#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class RosenbrockFunctionRotated : public BlackBoxOptimisationBenchmark {
     public:
      explicit RosenbrockFunctionRotated(
          const arma::uword numberOfDimensions);

     protected:
      const double max_;
    };
  }
}
