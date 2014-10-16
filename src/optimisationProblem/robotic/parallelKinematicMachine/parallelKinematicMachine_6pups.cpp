#include <hop_bits/optimisationProblem/robotic/parallelKinematicMachine/parallelKinematicMachine_6pups.hpp>

#include <hop_bits/helper/geometry.hpp>

namespace hop {
  ParallelKinematicMachine_6PUPS::ParallelKinematicMachine_6PUPS(const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints, const arma::Mat<double>::fixed<3, 6>& redundantJointStarts, const arma::Mat<double>::fixed<3, 6>& redundantJointEnds)
    : endEffectorJointsRelative_(relativeEndEffectorJoints),
      redundantJointStarts_(redundantJointStarts),
      redundantJointEnds_(redundantJointEnds),
      redundantJointStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
      redundantJointIndicies_(arma::find(arma::max(redundantJointStartToEnd_))) {

  }

  arma::Mat<double> ParallelKinematicMachine_6PUPS::getJacobian(const arma::Col<double>& endEffectorPose, const arma::Col<double>& redundantActuationParameters) const {
    if(any(redundantActuationParameters < 0) || any(redundantActuationParameters > 1)) {
      // TODO Add exception
    }

    arma::Col<double>::fixed<3> endEffector = endEffectorPose.subvec(0, 2);
    double endEffectorRollAngle = endEffectorPose.at(3);
    double endEffectorPitchAngle = endEffectorPose.at(4);
    double endEffectorYawAngle = endEffectorPose.at(5);

    arma::Mat<double>::fixed<3, 6> baseJoints = redundantJointStarts_;
    baseJoints.rows(redundantJointIndicies_) += redundantActuationParameters * redundantJointStartToEnd_;

    arma::Mat<double>::fixed<3, 6> endEffectorJointsRotated = Geometry::get3DRotationMatrix(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointsRelative_;
    arma::Mat<double>::fixed<3, 6> endEffectorJoints = endEffector + endEffectorJointsRotated;

    arma::Mat<double>::fixed<3, 6> baseToEndEffectorsJoints = endEffectorJoints - baseJoints;

    arma::Mat<double>::fixed<6, 6> forwardKinematic;
    for (std::size_t i = 0; i < forwardKinematic.n_rows; ++i) {
//      forwardKinematic.rows(i, arma::span(0, 2)) = baseToEndEffectorsJoints.row(i);
//      forwardKinematic.rows(i, arma::span(3, 5)) = arma::cross(endEffectorJointsRotated, aseToEndEffectorsJoints.row(i));
    }

    arma::Mat<double> inverseKinematic(6, 6 + redundantJointIndicies_.n_elem);
    inverseKinematic.diag() = static_cast<arma::Col<double>>(-arma::norm(baseToEndEffectorsJoints));
    for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
      arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
//      inverseKinematic.at(n, 6 + n) = (forwardKinematic.at(0, redundantJointIndex) * std::cos(redundantJointAngles_.at(redundantJointIndex) + forwardKinematic.at(1, redundantJointIndex) * std::sin(redundantJointAngles_.at(redundantJointIndex))));
    }

    return arma::solve(forwardKinematic, inverseKinematic);
  }

  arma::Mat<double>::fixed<3, 6> ParallelKinematicMachine_6PUPS::getRelativeEndEffectorJoints() const {
    return endEffectorJointsRelative_;
  }

  arma::Mat<double>::fixed<3, 6> ParallelKinematicMachine_6PUPS::getRedundantJointStarts() const {
    return redundantJointStarts_;
  }

  arma::Mat<double>::fixed<3, 6> ParallelKinematicMachine_6PUPS::getRedundantJointEnds() const {
    return redundantJointEnds_;
  }
}
