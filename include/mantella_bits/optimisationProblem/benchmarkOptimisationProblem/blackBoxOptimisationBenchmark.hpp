#pragma once
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#if defined(SUPPORT_MPI) // IWYU pragma: keep
#include <vector>
#endif

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem.hpp"

namespace mant {
  namespace bbob {
    class BlackBoxOptimisationBenchmark : public BenchmarkOptimisationProblem {
     public:
      explicit BlackBoxOptimisationBenchmark(
          const arma::uword numberOfDimensions);

     protected:
      arma::Col<double> getRandomParameterTranslation();

      arma::Col<double> getParameterConditioning(
          const double conditionNumber);

      arma::Col<double> getConditionedParameter(
          const arma::Col<double>& parameter);

      arma::Col<double> getAsymmetricParameter(
          const double asymmetry,
          const arma::Col<double>& parameter);

      arma::Col<double> getOscillatedParameter(
          const arma::Col<double>& parameter);

      arma::Mat<double> getRandomLocalParameterMediumConditionings(
          const arma::uword numberOfConditionings);

      arma::Mat<double> getRandomLocalParameterHighConditionings(
          const arma::uword numberOfConditionings);

      double getRandomObjectiveValueTranslation();

      double getOscillatedValue(
          const double oscilliation);

      double getBoundaryPenalty(
          const arma::Col<double>& parameter);
    };
  }
}
