#pragma once

// C++ standard library
#include <string>
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/config.hpp>
#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp>

namespace mant {
  namespace bbob {
    class SphereFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit SphereFunction(
          const arma::uword numberOfDimensions);

      std::string toString() const override;
#if defined(SUPPORT_MPI)
      std::vector<double> serialise() const;
      void deserialise(
          std::vector<double> serialisedOptimisationProblem);
#endif

     protected:
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
    };
  }
}
