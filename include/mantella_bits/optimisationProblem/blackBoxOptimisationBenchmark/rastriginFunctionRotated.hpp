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
    class RastriginFunctionRotated : public BlackBoxOptimisationBenchmark {
      public:
        explicit RastriginFunctionRotated(
            const arma::uword numberOfDimensions);

        void setRotationR(
            const arma::Mat<double>& rotationR);

        void setRotationQ(
            const arma::Mat<double>& rotationQ);

        std::string toString() const override;
        std::vector<double> serialise() const;
        void deserialise(
            std::vector<double> serialisedOptimisationProblem);

      protected:
        const arma::Col<double> parameterConditioning_;

        arma::Mat<double> rotationR_;
        arma::Mat<double> rotationQ_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const override;
    };
  }
}
