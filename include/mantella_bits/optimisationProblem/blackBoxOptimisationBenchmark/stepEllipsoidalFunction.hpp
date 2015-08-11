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
    class StepEllipsoidalFunction : public BlackBoxOptimisationBenchmark {
      public:
        explicit StepEllipsoidalFunction(
            const arma::uword numberOfDimensions);

        void setRotationQ(
            const arma::Mat<double>& rotationQ);

        std::string toString() const override;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        std::vector<double> serialise() const;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        void deserialise(
            std::vector<double> serialisedOptimisationProblem);

      protected:
        const arma::Col<double> firstParameterConditioning_;
        const arma::Col<double> secondParameterConditioning_;

        arma::Mat<double> rotationQ_;

        double getSoftConstraintsValueImplementation(
            const arma::Col<double>& parameter) const override;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const override;
    };
  }
}
