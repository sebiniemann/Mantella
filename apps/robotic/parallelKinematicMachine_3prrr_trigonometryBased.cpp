#include <parallelKinematicMachine_3prrr_trigonometryBased.hpp>

#include <cmath>

ParallelKinematicMachine_3PRRR_TrigonometryBased::ParallelKinematicMachine_3PRRR_TrigonometryBased()
  : ParallelKinematicMachine_3PRRR_TrigonometryBased(
      arma::Mat<double>::fixed<2, 3>({-0.000066580445834, 0.106954081945581, -0.092751709777083, -0.053477040972790, 0.092818290222917, -0.053477040972790}),
      arma::Mat<double>::fixed<2, 3>({0.6, 0.6, 0.6, 0.6, 0.6, 0.6}),
      arma::Mat<double>::fixed<2, 3>({0.1, 1.0392, 0.0, 0.8, 1.2, 0.8}),
      arma::Mat<double>::fixed<2, 3>({1.1, 1.0392, 0.0, -0.2, 1.2, -0.2})) {

}

ParallelKinematicMachine_3PRRR_TrigonometryBased::ParallelKinematicMachine_3PRRR_TrigonometryBased(const arma::Mat<double>::fixed<2, 3>& relativeEndEffectorJoints, const arma::Mat<double>::fixed<2, 3>& linkLengths, const arma::Mat<double>::fixed<2, 3>& redundantJointStarts, const arma::Mat<double>::fixed<2, 3>& redundantJointEnds)
  : endEffectorJointsRelative_(relativeEndEffectorJoints),
    linkLengths_(linkLengths),
    redundantJointStarts_(redundantJointStarts),
    redundantJointEnds_(redundantJointEnds),
    redundantJointsStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
    redundantJointIndicies_(arma::find(arma::any(redundantJointsStartToEnd_))),
    redundantJointAnglesSine_(redundantJointIndicies_.n_elem),
    redundantJointAnglesCosine_(redundantJointIndicies_.n_elem) {
  for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
    double redundantJointAngle = std::atan2(redundantJointsStartToEnd_.at(2, n), redundantJointsStartToEnd_.at(1, n));
    redundantJointAnglesSine_.at(n) = std::sin(redundantJointAngle);
    redundantJointAnglesCosine_.at(n) = std::cos(redundantJointAngle);
  }
}

arma::Mat<double> ParallelKinematicMachine_3PRRR_TrigonometryBased::getJacobian(const arma::Col<double>& endEffectorPose, const arma::Col<double>& redundantActuationParameters) const {
  if(any(redundantActuationParameters < 0) || any(redundantActuationParameters > 1)) {
    // TODO Add exception
  }

  arma::Col<double>::fixed<2> endEffector = endEffectorPose.subvec(0, 1);
  double endEffectorAngle = endEffectorPose.at(2);

  arma::Mat<double>::fixed<2, 3> baseJoints = redundantJointStarts_;
  for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
    std::size_t redundantJointIndex = redundantJointIndicies_.at(n);
    baseJoints.col(redundantJointIndex) += redundantActuationParameters.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
  }

//  std::cout << baseJoints << std::endl;

  arma::Mat<double>::fixed<2, 3> endEffectorJointsRotated = hop::Geometry::get2DRotationMatrix(endEffectorAngle) * endEffectorJointsRelative_;
  arma::Mat<double>::fixed<2, 3> endEffectorJoints = endEffectorJointsRotated;
  endEffectorJoints.each_col() += endEffector;

//  std::cout << endEffectorJoints << std::endl;

  arma::Mat<double>::fixed<2, 3> baseJointsToEndEffectorJoints = endEffectorJoints - baseJoints;

  arma::Row<double>::fixed<3> passiveAngles = arma::acos((arma::sum(arma::square(baseJointsToEndEffectorJoints)) - arma::sum(arma::square(linkLengths_))) / (2 * arma::prod(linkLengths_)));

//  std::cout << passiveAngles << std::endl;

  arma::Row<double>::fixed<3> activeJoints;
  for(std::size_t n; n < activeJoints.n_elem; ++n) {
    activeJoints.at(n) = std::atan2(
      (linkLengths_.at(0, n) + linkLengths_.at(0, n) * std::cos(passiveAngles.at(n))) * baseJointsToEndEffectorJoints.at(1, n) - (linkLengths_.at(1, n) * std::sin(passiveAngles.at(n))) * baseJointsToEndEffectorJoints.at(0, n),
      (linkLengths_.at(0, n) + linkLengths_.at(0, n) * std::cos(passiveAngles.at(n))) * baseJointsToEndEffectorJoints.at(0, n) + (linkLengths_.at(1, n) * std::sin(passiveAngles.at(n))) * baseJointsToEndEffectorJoints.at(1, n)
    );
  }

//  std::cout << activeJoints << std::endl;

  arma::Mat<double>::fixed<3, 3> forwardKinematic;
  forwardKinematic.row(0) = baseJointsToEndEffectorJoints.row(0) - linkLengths_.row(0) % arma::cos(activeJoints);
  forwardKinematic.row(1) = baseJointsToEndEffectorJoints.row(1) - linkLengths_.row(1) % arma::sin(activeJoints);
  forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointsRotated.row(0);

//  std::cout << forwardKinematic << std::endl;

  arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
  inverseKinematic.diag() = linkLengths_.row(0) % (forwardKinematic.row(0) % arma::sin(activeJoints) - forwardKinematic.row(1) % arma::cos(activeJoints));
  for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
    arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
    inverseKinematic.at(n, 3 + n) = -(forwardKinematic.at(redundantJointIndex, 0) * redundantJointAnglesCosine_.at(n) + forwardKinematic.at(redundantJointIndex, 1) * redundantJointAnglesSine_.at(n));
  }

//  std::cout << inverseKinematic << std::endl;

  return arma::solve(forwardKinematic.t(), inverseKinematic);
}
