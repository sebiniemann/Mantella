#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class LunacekBiRastriginFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit LunacekBiRastriginFunction(
          const arma::uword numberOfDimensions);

     protected:
      const double s_;
      const double mu_;
      const arma::Col<double> parameterConditinong_;
      // Cannot be *const*, as it is shared/set over MPI, to synchronise all problems.
      arma::Mat<double> rotationR_;
      arma::Mat<double> rotationQ_;
    };
  }
}
