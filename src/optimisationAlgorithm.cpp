#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  OptimisationAlgorithm::OptimisationAlgorithm() {

  }

  arma::Col<double> OptimisationAlgorithm::optimise(std::shared_ptr<OptimisationProblem> optimisationProblem) {
    return nullptr;
  }

  bool OptimisationAlgorithm::isFinished() const {
    return true;
  }

  bool OptimisationAlgorithm::isTerminated() const {
    return true;
  }

  void OptimisationAlgorithm::initialisation() {

  }

  void OptimisationAlgorithm::updateInformations(arma::Mat<double> parameters, arma::Mat<double> objectiveValues, arma::Mat<double> softConstraintsValues) {

  }
}
