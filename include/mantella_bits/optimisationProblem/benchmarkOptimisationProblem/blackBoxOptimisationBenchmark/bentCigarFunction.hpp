#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class BentCigarFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit BentCigarFunction(
          const arma::uword numberOfDimensions);

     protected:
      // Cannot be *const*, as it is shared/set over MPI, to synchronise all problems.
      arma::Mat<double> rotationQ_;
    };
  }
}
