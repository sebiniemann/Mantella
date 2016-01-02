#pragma once

// C++ standard library
#include <functional>
#include <unordered_map>

// Armadillo
namespace arma {
  template <typename T>
  class Col;
}

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual
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
