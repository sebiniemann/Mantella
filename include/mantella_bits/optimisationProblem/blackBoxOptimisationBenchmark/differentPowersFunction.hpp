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
    class DifferentPowersFunction : public BlackBoxOptimisationBenchmark {
      public:
        explicit DifferentPowersFunction(
            const arma::uword numberOfDimensions);

        std::string toString() const override;
        std::vector<double> serialise() const;
        void deserialise(
            std::vector<double> serialisedOptimisationProblem);

      protected:
        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const override;
    };
  }
}