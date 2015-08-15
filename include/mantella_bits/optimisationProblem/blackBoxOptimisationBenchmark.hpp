#pragma once

// C++ standard library
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationProblem.hpp>

namespace mant {
  namespace bbob {
    class BlackBoxOptimisationBenchmark : public OptimisationProblem {
      public:
        explicit BlackBoxOptimisationBenchmark(
            const arma::uword numberOfDimensions);

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        std::vector<double> serialise() const;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        void deserialise(
            std::vector<double> serialisedOptimisationProblem);

        virtual ~BlackBoxOptimisationBenchmark() = default;

      protected:
        arma::Col<double> getRandomParameterTranslation() const;

        arma::Col<double> getParameterConditioning(
            const double conditionNumber) const;

        arma::Col<double> getConditionedParameter(
            const arma::Col<double>& parameter) const;

        arma::Col<double> getAsymmetricParameter(
            const double asymmetry,
            const arma::Col<double>& parameter) const;
            
        double getOscillatedValue(
            const double oscilliation) const;

        arma::Col<double> getOscillatedParameter(
            const arma::Col<double>& parameter) const;

        double getBoundConstraintsValue(
            const arma::Col<double>& parameter) const;
    };
  }
}
