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
    class StepEllipsoidalFunction : public BlackBoxOptimisationBenchmark {
      public:
        explicit StepEllipsoidalFunction(
            const arma::uword numberOfDimensions);

        void setRotationQ(
            const arma::Mat<double>& rotationQ);

        std::string toString() const override;
#if defined(SUPPORT_MPI)
        std::vector<double> serialise() const;
        void deserialise(
            std::vector<double> serialisedOptimisationProblem);
#endif

      protected:
        const arma::Col<double> firstParameterConditioning_;
        const arma::Col<double> secondParameterConditioning_;

        arma::Mat<double> rotationQ_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const override;
    };
  }
}
