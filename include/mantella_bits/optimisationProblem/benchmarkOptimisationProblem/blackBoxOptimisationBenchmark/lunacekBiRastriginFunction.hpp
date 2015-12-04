#pragma once
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#if defined(SUPPORT_MPI) // IWYU pragma: keep
#include <vector>
#endif

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
      // We avoid to set this to *const*, as its randomly filled and potentially shared/reset over MPI, to synchronise all problems.
      arma::Mat<double> rotationR_;
      arma::Mat<double> rotationQ_;
    };
  }
}