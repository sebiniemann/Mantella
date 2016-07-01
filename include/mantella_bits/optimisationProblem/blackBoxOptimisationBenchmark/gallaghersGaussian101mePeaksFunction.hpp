#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

namespace mant {
  namespace bbob {
    class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit GallaghersGaussian101mePeaksFunction(
          const arma::uword numberOfDimensions);

     protected:
      const arma::vec::fixed<101> weights_;
      // Keeps randomly set data non-`const`, to be changed within tests.
      arma::mat rotationQ_;
      arma::mat localParameterTranslations_;
      arma::mat localParameterConditionings_;
    };
  }
}
