#include "mantella_bits/optimisationProblem/roboticsOptimisationProblem.hpp"

// Mantella
#include "mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp"

namespace mant {
  namespace robotics {
    RoboticsOptimisationProblem::RoboticsOptimisationProblem(
        std::shared_ptr<RobotModel> robotModel)
        : OptimisationProblem(robotModel->numberOfRedundantJoints_),
          robotModel_(robotModel) {
    }

    void RoboticsOptimisationProblem::setEndEffectorTrajectory(
        const arma::Mat<double>& endEffectorTrajectory) {
      endEffectorTrajectory_ = endEffectorTrajectory;
    }

    arma::Mat<double> RoboticsOptimisationProblem::getEndEffectorTrajectory() const {
      return endEffectorTrajectory_;
    }
  }
}
