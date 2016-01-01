#pragma once

// C++ standard library
#include <functional>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  class SurrogateModel : public OptimisationProblem {
   public:
    using OptimisationProblem::OptimisationProblem;

    void setModelFunction(
        const std::function<std::function<double(const arma::Col<double>& parameter)>(const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples)>& modelFunction);

    void model(
        const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);

   protected:
    std::function<std::function<double(const arma::Col<double>& parameter)>(const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples)> modelFunction_;
  };
}
