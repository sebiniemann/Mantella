#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel/multiLevelStewartPlatform.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {
  namespace robotics {
    MultiLevelStewartPlatform::MultiLevelStewartPlatform()
        : MultiLevelStewartPlatform({ParallelKinematicMachine6PUPS(), ParallelKinematicMachine6PUPS(), ParallelKinematicMachine6PUPS(), ParallelKinematicMachine6PUPS()}) {
    }

    MultiLevelStewartPlatform::MultiLevelStewartPlatform(
        const std::vector<ParallelKinematicMachine6PUPS>& platformLevels)
        : RobotModel(6, 6 * platformLevels.size() - 6),
          platformLevels_(platformLevels),
          numberOfPlatformLevels_(platformLevels_.size()) {
      for (arma::uword n = 0; n < numberOfPlatformLevels_; ++n) {
        verify(platformLevels_.at(n).numberOfRedundantJoints_ == 0,
            "");  // TODO
      }
    }

    arma::Cube<double> MultiLevelStewartPlatform::getModelImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1));

      arma::Cube<double> model = platformLevels_.at(0).getModel(endEffectorPose, {});
      for (arma::uword n = 1; n < platformLevels_.size(); ++n) {
        arma::join_slices(model, platformLevels_.at(n).getModel(redundantJointsActuation.subvec(6 * n - 6, 6 * n - 1), {}));
      }

      return model;
    }

    arma::Row<double> MultiLevelStewartPlatform::getActuationImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1));

      arma::Row<double> actuation = platformLevels_.at(0).getActuation(endEffectorPose, {});
      for (arma::uword n = 1; n < platformLevels_.size(); ++n) {
        const arma::Row<double>& partialActuation = platformLevels_.at(n).getActuation(redundantJointsActuation.subvec(6 * n - 6, 6 * n - 1), {});

        assert(partialActuation.n_elem == numberOfActiveJoints_);
        assert(!arma::any(partialActuation < platformLevels_.at(n).minimalActiveJointsActuation_) && !arma::any(partialActuation > platformLevels_.at(n).maximalActiveJointsActuation_));

        actuation = arma::join_rows(actuation, partialActuation);
      }

      return actuation;
    }

    double MultiLevelStewartPlatform::getEndEffectorPoseErrorImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < 0) && !arma::any(redundantJointsActuation > 1));

      double endEffectorPoseError = platformLevels_.at(0).getEndEffectorPoseError(endEffectorPose, {});
      for (arma::uword n = 1; n < platformLevels_.size(); ++n) {
        const double partialEndEffectorPoseError = platformLevels_.at(n).getEndEffectorPoseError(redundantJointsActuation.subvec(6 * n - 6, 6 * n - 1), {});

        assert(partialEndEffectorPoseError >= 0);

        endEffectorPoseError += partialEndEffectorPoseError;
      }

      return endEffectorPoseError;
    }

    std::string MultiLevelStewartPlatform::toString() const {
      return "multi_level_stewart_platform";
    }
  }
}
