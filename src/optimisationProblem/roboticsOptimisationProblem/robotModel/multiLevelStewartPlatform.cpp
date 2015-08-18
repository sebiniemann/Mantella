#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel/multiLevelStewartPlatform.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {
  namespace robotics {
    MultiLevelStewartPlatform::MultiLevelStewartPlatform(
        const arma::uword numberOfPlatformLevels,
        const arma::uword numberOfRedundantJoints) 
      : RobotModel(6 * numberOfPlatformLevels, numberOfRedundantJoints),
        numberOfPlatformLevels_(numberOfPlatformLevels) {
      for (arma::uword n = 0; n < numberOfPlatformLevels_; ++n) {
        platformLevels_.push_back(ParallelKinematicMachine6PUPS(0));
      }
    }

    void MultiLevelStewartPlatform::setPlatformLevels(
        const std::vector<ParallelKinematicMachine6PUPS>& platformLevels) {
      for (arma::uword n = 0; n < numberOfPlatformLevels_; ++n) {
        verify(platformLevels.at(n).numberOfRedundantJoints_ == 0, ""); // TODO
      }
      
      platformLevels_ = platformLevels;
    }

    arma::Cube<double> MultiLevelStewartPlatform::getModelImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < minimalRedundantJointsActuation_) && !arma::any(redundantJointsActuation > maximalRedundantJointsActuation_));
      
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
      assert(!arma::any(redundantJointsActuation < minimalRedundantJointsActuation_) && !arma::any(redundantJointsActuation > maximalRedundantJointsActuation_));
      
      arma::Row<double> actuation = platformLevels_.at(0).getActuation(endEffectorPose, {});
      for (arma::uword n = 1; n < platformLevels_.size(); ++n) {
        const arma::Row<double>& partialActuation = platformLevels_.at(n).getActuation(redundantJointsActuation.subvec(6 * n - 6, 6 * n - 1), {});
        
        assert(partialActuation.n_elem == numberOfActiveJoints_);
        assert(!arma::any(partialActuation < minimalActiveJointsActuation_) && !arma::any(partialActuation > maximalActiveJointsActuation_));
        
        actuation = arma::join_rows(actuation, partialActuation);
      }

      return actuation;
    }

    double MultiLevelStewartPlatform::getEndEffectorPoseErrorImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < minimalRedundantJointsActuation_) && !arma::any(redundantJointsActuation > maximalRedundantJointsActuation_));
      
      double endEffectorPoseError = platformLevels_.at(0).getEndEffectorPoseError(endEffectorPose, {});
      for (arma::uword n = 1; n < platformLevels_.size(); ++n) {
        const double& partialEndEffectorPoseError = platformLevels_.at(n).getEndEffectorPoseError(redundantJointsActuation.subvec(6 * n - 6, 6 * n - 1), {});
      
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
