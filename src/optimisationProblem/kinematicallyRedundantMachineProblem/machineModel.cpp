#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel.hpp"

// C++ standard library
#include <cassert>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  namespace krmp {
    MachineModel::MachineModel(
        const arma::uword numberOfActiveJoints,
        const arma::uword numberOfRedundantJoints)
        : numberOfActiveJoints_(numberOfActiveJoints),
          numberOfRedundantJoints_(numberOfRedundantJoints) {
    }

    void MachineModel::setModelFunction(
        const std::function<arma::Cube<double>(const arma::Col<double>& endEffectorPose, const arma::Row<double>& redundantJointsActuation)> modelFunction) {
      modelFunction_ = modelFunction;
    }

    arma::Cube<double> MachineModel::getModel(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      // Using the *operator bool* to checks whether *modelFunction_* is empty (not callable) or not.
      verify(static_cast<bool>(modelFunction_), "The objective function is not callable.");
      verify(redundantJointsActuation.n_elem == numberOfRedundantJoints_, "The number of actuated redundant joints must be equal to the number of redundant joints.");

      return modelFunction_(endEffectorPose, redundantJointsActuation);
    }

    void MachineModel::setPoseInaccuracyFunction(
        const std::function<double(const arma::Col<double>& endEffectorPose, const arma::Row<double>& redundantJointsActuation)> poseInaccuracyFunction) {
      poseInaccuracyFunction_ = poseInaccuracyFunction;
    }

    double MachineModel::getPoseInaccuracy(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      // Using the *operator bool* to checks whether *poseInaccuracyFunction_* is empty (not callable) or not.
      verify(static_cast<bool>(poseInaccuracyFunction_), "The objective function is not callable.");
      verify(redundantJointsActuation.n_elem == numberOfRedundantJoints_, "The number of actuated redundant joints must be equal to the number of redundant joints.");

      const double poseInaccuracy = poseInaccuracyFunction_(endEffectorPose, redundantJointsActuation);
      // The pose has the most accuracy, if the inaccuracy is 0. A negative inaccuracy wouldn't make any sense.
      assert(poseInaccuracy >= 0);

      return poseInaccuracy;
    }
  }
}
