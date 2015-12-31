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
    class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit GallaghersGaussian101mePeaksFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::Col<double>::fixed<101> weight_;
      // We avoid to set this to *const*, as its randomly filled and potentially shared/reset over MPI, to synchronise all problems.
      arma::Mat<double> localParameterConditionings_;
      arma::Mat<double> localParameterTranslations_;
      arma::Mat<double> rotationQ_;
    };
  }
}
