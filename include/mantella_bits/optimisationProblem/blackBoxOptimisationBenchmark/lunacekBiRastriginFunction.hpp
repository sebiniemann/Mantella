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
    class LunacekBiRastriginFunction : public BlackBoxOptimisationBenchmark {
      public:
        explicit LunacekBiRastriginFunction(
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
        const double s_;
        const double mu_;
        const arma::Col<double> parameterConditinong_;

        arma::Mat<double> rotationR_;
        arma::Mat<double> rotationQ_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const override;
    };
  }
}
