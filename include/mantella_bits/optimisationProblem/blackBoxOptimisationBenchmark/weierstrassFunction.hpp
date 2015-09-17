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
    class WeierstrassFunction : public BlackBoxOptimisationBenchmark {
     public:
      explicit WeierstrassFunction(
          const arma::uword numberOfDimensions);

      void setRotationR(
          const arma::Mat<double>& rotationR);

      void setRotationQ(
          const arma::Mat<double>& rotationQ);

      std::string toString() const override;
#if defined(SUPPORT_MPI)
      std::vector<double> serialise() const;
      void deserialise(
          std::vector<double> serialisedOptimisationProblem);
#endif

     protected:
      const double f0_;
      const arma::Col<double> parameterConditioning_;

      arma::Mat<double> rotationR_;
      arma::Mat<double> rotationQ_;

      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const override;
    };
  }
}
