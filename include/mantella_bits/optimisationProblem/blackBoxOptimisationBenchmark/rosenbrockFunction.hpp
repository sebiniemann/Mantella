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
    class RosenbrockFunction : public BlackBoxOptimisationBenchmark {
      public:
        explicit RosenbrockFunction(
            const arma::uword numberOfDimensions);

        std::string toString() const override;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        std::vector<double> serialise() const;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        void deserialise(
            std::vector<double> serialisedOptimisationProblem);

      protected:
        const double max_;

        arma::Col<double> localParameterTranslation_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const override;
    };
  }
}