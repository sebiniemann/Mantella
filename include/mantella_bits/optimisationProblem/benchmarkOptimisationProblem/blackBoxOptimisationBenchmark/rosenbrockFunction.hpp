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
    class RosenbrockFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit RosenbrockFunction(
          const arma::uword numberOfDimensions);

     protected:
      const double max_;
    };
  }
}
