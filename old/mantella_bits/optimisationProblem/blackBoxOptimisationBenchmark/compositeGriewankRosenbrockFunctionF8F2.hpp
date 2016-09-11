#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class CompositeGriewankRosenbrockFunctionF8F2 : public BlackBoxOptimisationBenchmark {
     public:
      explicit CompositeGriewankRosenbrockFunctionF8F2(
          const arma::uword numberOfDimensions);

     protected:
      const double max_;
    };
  }
}
