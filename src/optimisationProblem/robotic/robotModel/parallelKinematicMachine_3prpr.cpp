#include <hop_bits/optimisationProblem/robotic/robotModel/parallelKinematicMachine_3prpr.hpp>

#include <hop_bits/helper/geometry.hpp>

// TODO Implement limitations for each joint (min/max prismatic length, max angle, ...)
namespace hop {
  ParallelKinematicMachine_3PRPR::ParallelKinematicMachine_3PRPR()
    : ParallelKinematicMachine_3PRPR(
        arma::Mat<double>::fixed<2, 3>({-0.000066580445834, 0.106954081945581, -0.092751709777083, -0.053477040972790, 0.092818290222917, -0.053477040972790}),
        arma::Mat<double>::fixed<2, 3>({0.1, 1.0392, 0.0, 0.8, 1.2, 0.8}),
        arma::Mat<double>::fixed<2, 3>({1.1, 1.0392, 0.0, -0.2, 1.2, -0.2})) {

  }

  ParallelKinematicMachine_3PRPR::ParallelKinematicMachine_3PRPR(const arma::Mat<double>::fixed<2, 3>& relativeEndEffectorJoints, const arma::Mat<double>::fixed<2, 3>& redundantJointStarts, const arma::Mat<double>::fixed<2, 3>& redundantJointEnds)
    : endEffectorJointsRelative_(relativeEndEffectorJoints),
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

  arma::Mat<double> ParallelKinematicMachine_3PRPR::getJacobian(const arma::Col<double>& endEffectorPose, const arma::Col<double>& redundantActuationParameters) const {
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

    arma::Mat<double>::fixed<2, 3> endEffectorJointsRotated = hop::Geometry::get2DRotationMatrix(endEffectorAngle) * endEffectorJointsRelative_;
    arma::Mat<double>::fixed<2, 3> endEffectorJoints = endEffectorJointsRotated;
    endEffectorJoints.each_col() += endEffector;

    arma::Mat<double>::fixed<2, 3> baseToEndEffectorJoints = endEffectorJoints - baseJoints;
    
    arma::Mat<double>::fixed<3, 3> forwardKinematic;
    forwardKinematic.rows(0, 1) = baseToEndEffectorJoints;
    forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointsRotated.row(0);

    arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
    inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJoints)));
    for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
      arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
      inverseKinematic.at(n, 3 + n) = forwardKinematic.at(redundantJointIndex, 0) * redundantJointAnglesCosine_.at(n) + forwardKinematic.at(redundantJointIndex, 1) * redundantJointAnglesSine_.at(n);
    }

    return arma::solve(forwardKinematic.t(), inverseKinematic);
  }
}
