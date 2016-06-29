#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class EllipsoidalFunctionRotated : public BlackBoxOptimisationBenchmark {
     public:
      explicit EllipsoidalFunctionRotated(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec parameterConditioning_;
    };
  }
}
