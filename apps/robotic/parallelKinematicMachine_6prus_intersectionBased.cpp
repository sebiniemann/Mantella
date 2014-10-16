#include <parallelKinematicMachine_6prus_intersectionBased.hpp>

#include <hop_bits/helper/geometry.hpp>

ParallelKinematicMachine_6PRUS_IntersectionBased::ParallelKinematicMachine_6PRUS_IntersectionBased()
  : ParallelKinematicMachine_6PRUS_IntersectionBased(
      arma::Mat<double>::fixed<3, 6>({-0.000066580445834, 0.106954081945581, -0.092751709777083, -0.053477040972790, 0.092818290222917, -0.053477040972790}),
      arma::Mat<double>::fixed<3, 6>({0.6, 0.6, 0.6, 0.6, 0.6, 0.6}),
      arma::Mat<double>::fixed<3, 6>({0.1, 1.0392, 0.0, 0.8, 1.2, 0.8}),
      arma::Mat<double>::fixed<3, 6>({1.1, 1.0392, 0.0, -0.2, 1.2, -0.2})) {

}

ParallelKinematicMachine_6PRUS_IntersectionBased::ParallelKinematicMachine_6PRUS_IntersectionBased(const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints, const arma::Mat<double>::fixed<3, 6>& linkLengths, const arma::Mat<double>::fixed<3, 6>& redundantJointStarts, const arma::Mat<double>::fixed<3, 6>& redundantJointEnds)
  : endEffectorJointsRelative_(relativeEndEffectorJoints),
    linkLengths_(linkLengths),
    redundantJointStarts_(redundantJointStarts),
    redundantJointEnds_(redundantJointEnds),
    redundantJointsStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
    redundantJointIndicies_(arma::find(arma::any(redundantJointsStartToEnd_))),
    redundantJointAnglesSine_(redundantJointIndicies_.n_elem),
    redundantJointAnglesCosine_(redundantJointIndicies_.n_elem) {
  for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
    double redundantJointAngle = std::atan2(redundantJointsStartToEnd_.at(1, n), redundantJointsStartToEnd_.at(0, n));
    redundantJointAnglesSine_.at(n) = std::sin(redundantJointAngle);
    redundantJointAnglesCosine_.at(n) = std::cos(redundantJointAngle);
  }
}

arma::Mat<double> ParallelKinematicMachine_6PRUS_IntersectionBased::getJacobian(const arma::Col<double>& endEffectorPose, const arma::Col<double>& redundantActuationParameters) const {
  if(any(redundantActuationParameters < 0) || any(redundantActuationParameters > 1)) {
    // TODO Add exception
  }

  arma::Col<double>::fixed<2> endEffector = endEffectorPose.subvec(0, 1);
  double endEffectorAngle = endEffectorPose.at(2);

  arma::Mat<double>::fixed<3, 6> baseJoints = redundantJointStarts_;
  for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
    std::size_t redundantJointIndex = redundantJointIndicies_.at(n);
    baseJoints.col(redundantJointIndex) += redundantActuationParameters.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
  }

  arma::Mat<double>::fixed<3, 6> endEffectorJointsRotated = hop::Geometry::get2DRotationMatrix(endEffectorAngle) * endEffectorJointsRelative_;
  arma::Mat<double>::fixed<3, 6> endEffectorJoints = endEffectorJointsRotated;
  endEffectorJoints.each_col() += endEffector;

  arma::Mat<double>::fixed<3, 6> passiveJoints = hop::Geometry::getCircleCircleIntersection(baseJoints, linkLengths_.row(0), endEffectorJoints, linkLengths_.row(1));

  arma::Mat<double>::fixed<3, 3> forwardKinematic;
  forwardKinematic.row(0) = endEffectorJoints.row(0) - passiveJoints.row(0);
  forwardKinematic.row(1) = endEffectorJoints.row(1) - passiveJoints.row(1);
  forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointsRotated.row(0);

  arma::Mat<double>::fixed<3, 6> baseJointsToPassiveJoints = passiveJoints - baseJoints;
  arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
  inverseKinematic.diag() = forwardKinematic.row(0) % baseJointsToPassiveJoints.row(1) - forwardKinematic.row(1) % baseJointsToPassiveJoints.row(0);
  for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
    arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
    inverseKinematic.at(n, 3 + n) = -(forwardKinematic.at(redundantJointIndex, 0) * redundantJointAnglesCosine_.at(n) + forwardKinematic.at(redundantJointIndex, 1) * redundantJointAnglesSine_.at(n));
  }

  return arma::solve(forwardKinematic.t(), inverseKinematic);
}
