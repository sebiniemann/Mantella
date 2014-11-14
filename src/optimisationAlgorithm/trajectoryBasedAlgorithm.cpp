#include <hop_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>

namespace hop {
  void TrajectoryBasedAlgorithm::setInitialParameter(
      const arma::Col<double>& initialParameter) {
    initialParameter_ = initialParameter;
  }
}
