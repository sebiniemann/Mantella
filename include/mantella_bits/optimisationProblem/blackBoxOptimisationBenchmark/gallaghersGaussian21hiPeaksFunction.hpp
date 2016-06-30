#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class GallaghersGaussian21hiPeaksFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit GallaghersGaussian21hiPeaksFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec::fixed<21> weights_;
      // Keeps randomly set data non-`const`, to be changed within tests.
      arma::mat rotationQ_;
      arma::mat localParameterTranslations_;
      arma::mat localParameterConditionings_;
    };
  }
}
