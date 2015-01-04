#include <hop_bits/optimisationProblem/nkModel.hpp>

namespace hop {
  NKModel::NKModel(
      const unsigned int& numberOfNeighbours,
      const unsigned int& numberOfDimensions) noexcept
    : OptimisationProblem(numberOfDimensions){

  }

  double NKModel::getObjectiveValueImplementation(
      const arma::Col<unsigned int>& parameter) const noexcept {
    return arma::sum(parameter);
  }
}
