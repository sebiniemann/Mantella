#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  namespace krm {
    KinematicallyRedundantMachines::KinematicallyRedundantMachines(
        const arma::uword numberOfProblemDimensions,
        const arma::uword numberOfWorkspaceDimensions)
        : OptimisationProblem(numberOfProblemDimensions),
          numberOfWorkspaceDimensions_(numberOfWorkspaceDimensions) {
      setEndEffectorTrajectory(arma::zeros<arma::Mat<double>>(numberOfWorkspaceDimensions_, 1));
    }

    void KinematicallyRedundantMachines::setEndEffectorTrajectory(
        const arma::Mat<double>& endEffectorTrajectory) {
      verify(endEffectorTrajectory.n_rows == numberOfWorkspaceDimensions_, "KinematicallyRedundantMachines.setEndEffectorTrajectory: The end-effector trajectory's number of rows must be equal to the workspace's number of dimensions.");
      verify(endEffectorTrajectory.n_cols > 0, "KinematicallyRedundantMachines.setEndEffectorTrajectory: The end-effector trajectory's number of columns must be strict greater than 0.");
      verify(endEffectorTrajectory.is_finite(), "KinematicallyRedundantMachines.setEndEffectorTrajectory: The end-effector trajectory must be finite.");

      endEffectorTrajectory_ = endEffectorTrajectory;
#if defined(SUPPORT_MPI)
      MPI_Bcast(endEffectorTrajectory_.memptr(), static_cast<int>(endEffectorTrajectory_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

      reset();
    }

    arma::Mat<double> KinematicallyRedundantMachines::getEndEffectorTrajectory() const {
      return endEffectorTrajectory_;
    }
  }
}
