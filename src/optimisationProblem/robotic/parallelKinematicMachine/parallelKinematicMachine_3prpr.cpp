#include <hop_bits/optimisationProblem/robotic/parallelKinematicMachine/parallelKinematicMachine_3prpr.hpp>

#include <hop_bits/helper/geometry.hpp>

namespace hop {
  ParallelKinematicMachine_3PRPR::ParallelKinematicMachine_3PRPR(const arma::Mat<double>::fixed<2, 3>& relativeEndEffectorJoints, const arma::Mat<double>::fixed<2, 3>& redundantJointStarts, const arma::Mat<double>::fixed<2, 3>& redundantJointEnds)
    : endEffectorJointsRelative_(relativeEndEffectorJoints),
      redundantJointStarts_(redundantJointStarts),
      redundantJointEnds_(redundantJointEnds),
      redundantJointStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
      redundantJointIndicies_(arma::find(arma::max(redundantJointStartToEnd_))) {
    for(std::size_t n = 0; n < redundantJointAngles_.n_elem; ++n) {
      redundantJointAngles_.at(n) = std::atan2(redundantJointStartToEnd_.at(2, n), redundantJointStartToEnd_.at(1, n));
    }
  }

  arma::Mat<double> ParallelKinematicMachine_3PRPR::getJacobian(const arma::Col<double>& endEffectorPose, const arma::Col<double>& redundantActuationParameters) const {
    if(any(redundantActuationParameters < 0) || any(redundantActuationParameters > 1)) {
      // TODO Add exception
    }

    arma::Col<double>::fixed<2> endEffector = endEffectorPose.subvec(0, 1);
    double endEffectorAngle = endEffectorPose.at(2);

    arma::Mat<double>::fixed<2, 3> baseJoints = redundantJointStarts_;
    baseJoints.rows(redundantJointIndicies_) += redundantActuationParameters * redundantJointStartToEnd_;

    arma::Mat<double>::fixed<2, 3> endEffectorJointsRotated = Geometry::get2DRotationMatrix(endEffectorAngle) * endEffectorJointsRelative_;
    arma::Mat<double>::fixed<2, 3> endEffectorJoints = endEffector + endEffectorJointsRotated;

    arma::Mat<double>::fixed<2, 3> baseToEndEffectorsJoints = endEffectorJoints - baseJoints;

    arma::Mat<double>::fixed<3, 3> forwardKinematic;
    forwardKinematic.col(0) = baseToEndEffectorsJoints.row(0);
    forwardKinematic.col(1) = baseToEndEffectorsJoints.row(1);
    forwardKinematic.col(2) = -forwardKinematic.col(0) * endEffectorJointsRotated.row(0) + forwardKinematic.col(1) * endEffectorJointsRotated.row(1);

    arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem);
    inverseKinematic.diag() = static_cast<arma::Col<double>>(-arma::norm(baseToEndEffectorsJoints));
    for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
      arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
      inverseKinematic.at(n, 3 + n) = (forwardKinematic.at(0, redundantJointIndex) * std::cos(redundantJointAngles_.at(redundantJointIndex) + forwardKinematic.at(1, redundantJointIndex) * std::sin(redundantJointAngles_.at(redundantJointIndex))));
    }

    return arma::solve(forwardKinematic, inverseKinematic);
  }

  arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine_3PRPR::getRelativeEndEffectorJoints() const {
    return endEffectorJointsRelative_;
  }

  arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine_3PRPR::getRedundantJointStarts() const {
    return redundantJointStarts_;
  }

  arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine_3PRPR::getRedundantJointEnds() const {
    return redundantJointEnds_;
  }
}
