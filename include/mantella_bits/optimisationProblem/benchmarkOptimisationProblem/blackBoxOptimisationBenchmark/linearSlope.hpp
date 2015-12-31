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
    class LinearSlope : public BlackBoxOptimisationBenchmark {
     public:
      explicit LinearSlope(
          const arma::uword numberOfDimensions);

     protected:
      const arma::Col<double> parameterConditioning_;
      const double f0_;
    };
  }
}
