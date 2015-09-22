#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {
  namespace robotics {
    RobotModel::RobotModel(
        const arma::uword numberOfActiveJoints,
        const arma::uword numberOfRedundantJoints)
        : numberOfActiveJoints_(numberOfActiveJoints),
          numberOfRedundantJoints_(numberOfRedundantJoints) {
    }

    arma::Cube<double> RobotModel::getModel(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      verify(redundantJointsActuation.n_elem == numberOfRedundantJoints_,
          "The number of actuated redundant joints must be equal to the number of redundant joints.");
      verify(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1),
          "The redundant joints must be actuated within [0, 1].");

      return getModelImplementation(endEffectorPose, redundantJointsActuation);
    }

    arma::Row<double> RobotModel::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      verify(redundantJointsActuation.n_elem == numberOfRedundantJoints_,
          "The number of actuated redundant joints must be equal to the number of redundant joints.");
      verify(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1),
          "The redundant joints must be actuated within [0, 1].");

      const arma::Row<double>& actuation = getActuationImplementation(endEffectorPose, redundantJointsActuation);

      return actuation;
    }

    double RobotModel::getEndEffectorPoseError(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      verify(redundantJointsActuation.n_elem == numberOfRedundantJoints_,
          "The number of actuated redundant joints must be equal to the number of redundant joints.");
      verify(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1),
          "The redundant joints must be actuated within [0, 1].");

      const double endEffectorPoseError = getEndEffectorPoseErrorImplementation(endEffectorPose, redundantJointsActuation);
      assert(endEffectorPoseError >= 0);

      return endEffectorPoseError;
    }
  }
}
