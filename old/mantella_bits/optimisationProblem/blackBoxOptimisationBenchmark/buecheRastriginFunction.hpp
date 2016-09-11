#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class BuecheRastriginFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit BuecheRastriginFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec parameterConditioning_;
    };
  }
}
