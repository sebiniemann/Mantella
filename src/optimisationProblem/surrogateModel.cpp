#include "mantella_bits/optimisationProblem/surrogateModel.hpp"

// C++ standard library
#include <utility>

// Armadillo
#include <armadillo>

namespace mant {
  void SurrogateModel::setModelFunction(
      const std::function<std::function<double(const arma::Col<double>& parameter)>(const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples)>& modelFunction) {
    verify(static_cast<bool>(modelFunction), ""); // TODO

    modelFunction_ = modelFunction;
  }

  void SurrogateModel::model(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
    setObjectiveFunction(modelFunction_(samples));
  }
}
