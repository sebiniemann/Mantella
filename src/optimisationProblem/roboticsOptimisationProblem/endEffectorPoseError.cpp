#include "mantella_bits/optimisationProblem/roboticsOptimisationProblem/endEffectorPoseError.hpp"

namespace mant {
  namespace robotics {
    double EndEffectorPoseError::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      return 0.0;
    }

    std::string EndEffectorPoseError::toString() const {
      return "robotic_end_effector_pose_error";
    }
  }
}
