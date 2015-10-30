#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationProblem/surrogateModel.hpp>

namespace mant {
  class ArtificialNeuralNetwork : public SurrogateModel {
   public:
    using SurrogateModel::SurrogateModel;

    std::string toString() const override;

   protected:
    void modelImplementation() override;

    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override;
  };
}
