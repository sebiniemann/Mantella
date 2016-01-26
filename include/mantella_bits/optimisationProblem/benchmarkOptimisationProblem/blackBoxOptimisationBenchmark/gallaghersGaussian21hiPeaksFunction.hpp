#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class GallaghersGaussian21hiPeaksFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit GallaghersGaussian21hiPeaksFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::Col<double>::fixed<21> weight_;
      // Cannot be *const*, as it is shared/set over MPI, to synchronise all problems.
      arma::Mat<double> localParameterConditionings_;
      arma::Mat<double> localParameterTranslations_;
      arma::Mat<double> rotationQ_;
    };
  }
}
