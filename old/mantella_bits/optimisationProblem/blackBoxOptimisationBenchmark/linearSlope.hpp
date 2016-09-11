#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class LinearSlope : public BlackBoxOptimisationBenchmark {
     public:
      explicit LinearSlope(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec parameterConditioning_;
      const double f0_;
    };
  }
}
