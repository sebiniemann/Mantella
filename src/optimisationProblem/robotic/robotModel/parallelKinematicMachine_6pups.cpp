#include <hop_bits/optimisationProblem/robotic/robotModel/parallelKinematicMachine_6pups.hpp>

#include <hop_bits/helper/geometry.hpp>

// TODO Implement limitations for each joint (min/max prismatic length, max angle, ...)
namespace hop {
  ParallelKinematicMachine_6PUPS::ParallelKinematicMachine_6PUPS()
    : ParallelKinematicMachine_6PUPS(
        arma::Mat<double>::fixed<3, 6>({-0.000066580445834, 0.106954081945581, -0.092751709777083, -0.053477040972790, 0.092818290222917, -0.053477040972790}),
        arma::Mat<double>::fixed<3, 6>({0.1, 1.0392, 0.0, 0.8, 1.2, 0.8}),
        arma::Mat<double>::fixed<3, 6>({1.1, 1.0392, 0.0, -0.2, 1.2, -0.2})) {

  }

  ParallelKinematicMachine_6PUPS::ParallelKinematicMachine_6PUPS(const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints, const arma::Mat<double>::fixed<3, 6>& redundantJointStarts, const arma::Mat<double>::fixed<3, 6>& redundantJointEnds)
    : endEffectorJointsRelative_(relativeEndEffectorJoints),
      redundantJointStarts_(redundantJointStarts),
      redundantJointEnds_(redundantJointEnds),
      redundantJointsStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
      redundantJointIndicies_(arma::find(arma::any(redundantJointsStartToEnd_))),
      redundantJointAngles_(3, redundantJointIndicies_.n_elem) {
    for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
      double redundantJointXAngle = std::atan2(redundantJointsStartToEnd_.at(1, n), redundantJointsStartToEnd_.at(0, n));
      double redundantJointYAngle = std::atan2(redundantJointsStartToEnd_.at(2, n), redundantJointsStartToEnd_.at(1, n));
      redundantJointAngles_.col(n) = arma::Col<double>::fixed<3>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
    }
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
    for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
      std::size_t redundantJointIndex = redundantJointIndicies_.at(n);
      baseJoints.col(redundantJointIndex) += redundantActuationParameters.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
    }

    arma::Mat<double>::fixed<3, 6> endEffectorJointsRotated = hop::Geometry::get3DRotationMatrix(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointsRelative_;
    arma::Mat<double>::fixed<3, 6> endEffectorJoints = endEffectorJointsRotated;
    endEffectorJoints.each_col() += endEffector;

    arma::Mat<double>::fixed<3, 6> baseToEndEffectorJoints = endEffectorJoints - baseJoints;
    
    arma::Mat<double>::fixed<6, 6> forwardKinematic;
    forwardKinematic.rows(0, 2) = baseToEndEffectorJoints;
    for(std::size_t n = 0; n < 3; ++n) {
      forwardKinematic.row(3 + n) = arma::cross(endEffectorJointsRotated, baseToEndEffectorJoints.col(n));
    }

    arma::Mat<double> inverseKinematic(6, 6 + redundantJointIndicies_.n_elem, arma::fill::zeros);
    inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJoints)));
    for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
      arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
      inverseKinematic.at(n, 6 + n) = arma::dot(baseToEndEffectorJoints.col(redundantJointIndex), redundantJointAngles_.col(redundantJointIndex));
    }

    return arma::solve(forwardKinematic.t(), inverseKinematic);
  }
}
