#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  namespace krm {
    KinematicallyRedundantMachines::KinematicallyRedundantMachines(
        const arma::uword numberOfDimensions)
        : OptimisationProblem(numberOfDimensions),
          endEffectorTrajectory_(arma::zeros<arma::Mat<double>>(numberOfDimensions_)) {
    }

    void KinematicallyRedundantMachines::setEndEffectorTrajectory(
        const arma::Mat<double>& endEffectorTrajectory) {
      verify(endEffectorTrajectory.n_rows == numberOfDimensions_, ""); // TODO
      verify(endEffectorTrajectory.is_finite(), ""); // TODO

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
