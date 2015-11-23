#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/endEffectorPoseError.hpp"

namespace mant {
  namespace krmp {
    double EndEffectorPoseError::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      return 0.0;
    }

    std::string EndEffectorPoseError::toString() const {
      return "robotic_end_effector_pose_error";
    }
  }
}
