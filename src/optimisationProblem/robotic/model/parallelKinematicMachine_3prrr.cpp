#include <hop_bits/optimisationProblem/robotic/model/parallelKinematicMachine_3prrr.hpp>

// HOP
#include <hop_bits/helper/geometry.hpp>

namespace hop {
  namespace robotic {
    ParallelKinematicMachine_3PRRR::ParallelKinematicMachine_3PRRR()
      : ParallelKinematicMachine_3PRRR(
          arma::Mat<double>::fixed<2, 3>({
            -0.000066580445834, 0.106954081945581,
            -0.092751709777083, -0.053477040972790,
            0.092818290222917, -0.053477040972790}),
          arma::Mat<double>::fixed<2, 3>({
            0.6, 0.6,
            0.6, 0.6,
            0.6, 0.6}),
          arma::Mat<double>::fixed<2, 3>({
            0.1, 1.0392,
            0.0, 0.8,
            1.2, 0.8}),
          arma::Mat<double>::fixed<2, 3>({
            1.1, 1.0392,
            0.0, -0.2,
            1.2, -0.2})) {

    }

    ParallelKinematicMachine_3PRRR::ParallelKinematicMachine_3PRRR(const arma::Mat<double>::fixed<2, 3>& relativeEndEffectorJoints, const arma::Mat<double>::fixed<2, 3>& linkLengths, const arma::Mat<double>::fixed<2, 3>& redundantJointStarts, const arma::Mat<double>::fixed<2, 3>& redundantJointEnds)
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

    std::vector<arma::Mat<double>> ParallelKinematicMachine_3PRRR::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      std::vector<arma::Mat<double>> modelCharacterisation;

      if(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::runtime_error("All values for the actuation of redundantion joints must be between [0, 1].");
      }

      arma::Col<double>::fixed<2> endEffector = endEffectorPose.subvec(0, 1);
      double endEffectorAngle = endEffectorPose.at(2);

      arma::Mat<double>::fixed<2, 3> baseJoints = redundantJointStarts_;
      for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        std::size_t redundantJointIndex = redundantJointIndicies_.at(n);
        baseJoints.col(redundantJointIndex) += redundantJointActuations.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
      }

      arma::Mat<double>::fixed<2, 3> endEffectorJoints = hop::Geometry::get2DRotationMatrix(endEffectorAngle) * endEffectorJointsRelative_;
      endEffectorJoints.each_col() += endEffector;

      arma::Mat<double>::fixed<2, 3> passiveJoints;
      for(std::size_t n = 0; n < baseJoints.n_cols; ++n) {
        passiveJoints.col(n) = hop::Geometry::getCircleCircleIntersection(baseJoints.col(n), linkLengths_.at(0, n), endEffectorJoints.col(n), linkLengths_.at(1, n));
      }

      modelCharacterisation.push_back(baseJoints);
      modelCharacterisation.push_back(endEffectorJoints);
      modelCharacterisation.push_back(passiveJoints);

      return modelCharacterisation;
    }

    arma::Mat<double> ParallelKinematicMachine_3PRRR::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      std::vector<arma::Mat<double>> modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      arma::Mat<double>::fixed<2, 3> baseJointPositions = modelCharacterisation.at(0);
      arma::Mat<double>::fixed<2, 3> passiveJointPositions = modelCharacterisation.at(2);

      arma::Mat<double>::fixed<2, 3> baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<3> actuation;
      for(std::size_t n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation.at(n) = std::atan2(baseToPassiveJointPositions.at(1, n), baseToPassiveJointPositions.at(0, n));
      }

      return actuation;
    }

    double ParallelKinematicMachine_3PRRR::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      std::vector<arma::Mat<double>> modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      arma::Mat<double>::fixed<2, 3> baseJoints = modelCharacterisation.at(0);

      arma::Mat<double>::fixed<2, 3> endEffectorJoints = modelCharacterisation.at(1);
      arma::Mat<double>::fixed<2, 3> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      arma::Mat<double>::fixed<2, 3> passiveJoints = modelCharacterisation.at(2);

      arma::Mat<double>::fixed<3, 3> forwardKinematic;
      forwardKinematic.rows(0, 1) = endEffectorJoints - passiveJoints;
      forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointsRotated.row(0);

      arma::Mat<double>::fixed<2, 3> baseToPassiveJoints = passiveJoints - baseJoints;
      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % baseToPassiveJoints.row(1) - forwardKinematic.row(1) % baseToPassiveJoints.row(0);
      for(std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
        inverseKinematic.at(n, 3 + n) = -(forwardKinematic.at(redundantJointIndex, 0) * redundantJointAnglesCosine_.at(n) + forwardKinematic.at(redundantJointIndex, 1) * redundantJointAnglesSine_.at(n));
      }

      return -1 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
