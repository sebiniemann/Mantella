#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class EllipsoidalFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit EllipsoidalFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec parameterConditioning_;
    };
  }
}
