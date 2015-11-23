#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class StepEllipsoidalFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit StepEllipsoidalFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::Col<double> firstParameterConditioning_;
      const arma::Col<double> secondParameterConditioning_;
      // Cannot be *const*, as it is shared/set over MPI, to synchronise all problems.
      arma::Mat<double> rotationQ_;
    };
  }
}
