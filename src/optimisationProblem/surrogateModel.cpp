#include "mantella_bits/optimisationProblem/surrogateModel.hpp"

// C++ standard library
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  void SurrogateModel::setModelFunction(
      const std::function<std::function<double(const arma::Col<double>& parameter)>(const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples)>& modelFunction) {
    modelFunction_ = modelFunction;
  }

  void SurrogateModel::model(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
    // TODO Check that *modelFunction_* is callable
    setObjectiveFunction(modelFunction_(samples));
  }
}
