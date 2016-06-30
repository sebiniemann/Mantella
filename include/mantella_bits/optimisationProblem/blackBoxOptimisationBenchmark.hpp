#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  namespace bbob {
    class BlackBoxOptimisationBenchmark : public OptimisationProblem {
     public:
      explicit BlackBoxOptimisationBenchmark(
          const arma::uword numberOfDimensions);

     protected:
      arma::vec getRandomParameterTranslation();

      arma::vec getParameterConditioning(
          const double conditionNumber);

      arma::vec getConditionedParameter(
          const arma::vec& parameter);

      arma::vec getAsymmetricParameter(
          const double asymmetry,
          const arma::vec& parameter);

      arma::vec getOscillatedParameter(
          const arma::vec& parameter);

      double getRandomObjectiveValueTranslation();

      double getOscillatedValue(
          const double oscilliation);
    };
  }
}
