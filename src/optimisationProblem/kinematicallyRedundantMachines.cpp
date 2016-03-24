#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines.hpp"

// C++ standard library
#include <stdexcept>

// Mantella
#include "mantella_bits/mpi.hpp"

namespace mant {
  namespace krm {
    KinematicallyRedundantMachines::KinematicallyRedundantMachines(
        const arma::uword numberOfProblemDimensions,
        const arma::uword numberOfWorkspaceDimensions)
        : OptimisationProblem(numberOfProblemDimensions),
          numberOfWorkspaceDimensions_(synchronise(numberOfWorkspaceDimensions)) {
      if (numberOfWorkspaceDimensions_ == 0) {
        throw std::domain_error("KinematicallyRedundantMachines: The number of workspace dimensions must be greater than 0.");
      }

      // Initialises the end-effector trajectory with a single position at (0, ..., 0).
      setEndEffectorTrajectory(arma::zeros<arma::mat>(numberOfWorkspaceDimensions_, 1));
    }

    void KinematicallyRedundantMachines::setEndEffectorTrajectory(
        const arma::mat& endEffectorTrajectory) {
      if (endEffectorTrajectory.is_empty()) {
        throw std::invalid_argument("KinematicallyRedundantMachines.setEndEffectorTrajectory: The end-effector trajectory's number of columns must be greater than 0.");
      } else if (endEffectorTrajectory.n_rows != numberOfWorkspaceDimensions_) {
        throw std::invalid_argument("KinematicallyRedundantMachines.setEndEffectorTrajectory: The end-effector trajectory's number of rows must be equal to the workspace's number of dimensions.");
      }

      endEffectorTrajectory_ = synchronise(endEffectorTrajectory);

      reset();
    }

    arma::mat KinematicallyRedundantMachines::getEndEffectorTrajectory() const {
      return endEffectorTrajectory_;
    }
  }
}
