#pragma once

// C++ standard library
#include <string>
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp>

namespace mant {
  namespace bbob {
    class BuecheRastriginFunction : public BlackBoxOptimisationBenchmark {
      public:
        explicit BuecheRastriginFunction(
            const arma::uword numberOfDimensions);

        std::string toString() const override;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        std::vector<double> serialise() const;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        void deserialise(
            std::vector<double> serialisedOptimisationProblem);

      protected:
        const arma::Col<double> parameterConditioning_;

        double getSoftConstraintsValueImplementation(
            const arma::Col<double>& parameter) const override;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const override;
    };
  }
}
