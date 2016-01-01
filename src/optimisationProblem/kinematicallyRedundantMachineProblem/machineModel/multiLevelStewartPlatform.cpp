#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/multiLevelStewartPlatform.hpp"

// C++ standard library
#include <cassert>
// IWYU pragma: no_include <ext/alloc_traits.h>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  namespace krmp {
    MultiLevelStewartPlatform::MultiLevelStewartPlatform()
        : MultiLevelStewartPlatform{{ParallelKinematicMachine6PUPS(), ParallelKinematicMachine6PUPS(), ParallelKinematicMachine6PUPS(), ParallelKinematicMachine6PUPS()}} {
    }

    MultiLevelStewartPlatform::MultiLevelStewartPlatform(
        const std::vector<ParallelKinematicMachine6PUPS>& platformLevels)
        : MachineModel(6, 6 * platformLevels.size() - 6),
          platformLevels_(platformLevels),
          numberOfPlatformLevels_(platformLevels_.size()) {
      for (arma::uword n = 0; n < numberOfPlatformLevels_; ++n) {
        verify(platformLevels_.at(n).numberOfRedundantJoints_ == 0, ""); // TODO
      }
    }

    // arma::Cube<double> MultiLevelStewartPlatform::getModelImplementation(
    // const arma::Col<double>& endEffectorPose,
    // const arma::Row<double>& redundantJointsActuation) const {
    // assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
    // assert(arma::all(redundantJointsActuation >= 0) && arma::all(redundantJointsActuation <= 1));

    // arma::Cube<double> model = platformLevels_.at(0).getModel(endEffectorPose, {});
    // for (arma::uword n = 1; n < platformLevels_.size(); ++n) {
    // arma::join_slices(model, platformLevels_.at(n).getModel(redundantJointsActuation.subvec(6 * n - 6, 6 * n - 1), {}));
    // }

    // return model;
    // }

    // double MultiLevelStewartPlatform::getEndEffectorPoseErrorImplementation(
    // const arma::Col<double>& endEffectorPose,
    // const arma::Row<double>& redundantJointsActuation) const {
    // assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
    // assert(arma::all(redundantJointsActuation >= 0) && arma::all(redundantJointsActuation <= 1));

    // double endEffectorPoseError = platformLevels_.at(0).getEndEffectorPoseError(endEffectorPose, {});
    // for (arma::uword n = 1; n < platformLevels_.size(); ++n) {
    // const double partialEndEffectorPoseError = platformLevels_.at(n).getEndEffectorPoseError(redundantJointsActuation.subvec(6 * n - 6, 6 * n - 1), {});

    // assert(partialEndEffectorPoseError >= 0);

    // endEffectorPoseError += partialEndEffectorPoseError;
    // }

    // return endEffectorPoseError;
    // }
  }
}
