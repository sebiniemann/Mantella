#include <hop_bits/optimisationProblem/robotic/model/parallelKinematicMachine_4prus.hpp>

// HOP
#include <hop_bits/helper/geometry.hpp>

namespace hop {
  namespace robotic {
    ParallelKinematicMachine_4PRUS::ParallelKinematicMachine_4PRUS()
      : ParallelKinematicMachine_4PRUS(
          arma::Mat<double>::fixed<2, 4>({
                                           // TODO Recalculate (Delta Robot)
            0.0, 6.143558967020040,
            0.0, 0.139626340159546,
            0.0, 1.954768762233649,
            0.0, 2.234021442552742}),

          arma::Mat<double>::fixed<3, 4>({
            -0.027346281319362, 0.067684421383375, 0.0,
            0.027346281319362, 0.067684421383375, 0.0,
            0.072289569018135, -0.010159636370085, 0.0,
            0.044943287698773, -0.057524785013291, 0.0}),

          arma::Mat<double>::fixed<2, 4>({
            0.24, 0.56,
            0.24, 0.56,
            0.24, 0.56,
            0.24, 0.56}),

          arma::Mat<double>::fixed<3, 4>({
            -0.050659008749464, 0.360457577021932, -0.6,
            0.050659008749464, 0.360457577021932, -0.6,
            0.337494923062311, -0.136356800003392, -0.6,
            0.286835914312847, -0.224100777018540, -0.6}),

          arma::Mat<double>::fixed<3, 4>({
            -0.050659008749464, 0.360457577021932, 0.2,
            0.050659008749464, 0.360457577021932, 0.2,
            0.337494923062311, -0.136356800003392, 0.2,
            0.286835914312847, -0.224100777018540, 0.2})) {

    }

    ParallelKinematicMachine_4PRUS::ParallelKinematicMachine_4PRUS(
        const arma::Mat<double>::fixed<2, 4>& baseJointsAngles,
        const arma::Mat<double>::fixed<3, 4>& relativeEndEffectorJoints,
        const arma::Mat<double>::fixed<2, 4>& linkLengths,
        const arma::Mat<double>::fixed<3, 4>& redundantJointStarts,
        const arma::Mat<double>::fixed<3, 4>& redundantJointEnds)
      : endEffectorJointsRelative_(relativeEndEffectorJoints),
        linkLengths_(linkLengths),
        redundantJointStarts_(redundantJointStarts),
        redundantJointEnds_(redundantJointEnds),
        redundantJointsStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
        redundantJointIndicies_(arma::find(arma::any(redundantJointsStartToEnd_))),
        redundantJointAngles_(3, redundantJointIndicies_.n_elem) {
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        double redundantJointXAngle = std::atan2(redundantJointsStartToEnd_.at(1, n), redundantJointsStartToEnd_.at(0, n));
        double redundantJointYAngle = std::atan2(redundantJointsStartToEnd_.at(2, n), redundantJointsStartToEnd_.at(1, n));
        redundantJointAngles_.col(n) = arma::Col<double>::fixed<3>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }

      for (std::size_t n = 0; n < baseJointsAngles.n_cols; ++n) {
        baseJointsRotation_.slice(n) = get3DRotationMatrix(redundantJointAngles_.at(0, n), 0, redundantJointAngles_.at(1, n));
      }

      for (std::size_t n = 0; n < baseJointsRotation_.n_slices; ++n) {
        baseJointsNormal_.col(n) = arma::normalise(arma::cross(baseJointsRotation_.slice(n).col(0), baseJointsRotation_.slice(n).col(2)));
      }
    }

    std::vector<arma::Mat<double>> ParallelKinematicMachine_4PRUS::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      std::vector<arma::Mat<double>> modelCharacterisation;

      if (arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::runtime_error("All values for the actuation of redundantion joints must be between [0, 1].");
      }

      arma::Col<double>::fixed<3> endEffectorPosition = endEffectorPose.subvec(0, 2);
      double endEffectorRollAngle = endEffectorPose.at(3);
      double endEffectorPitchAngle = endEffectorPose.at(4);
      double endEffectorYawAngle = endEffectorPose.at(5);

      arma::Mat<double>::fixed<3, 4> baseJoints = redundantJointStarts_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        std::size_t redundantJointIndex = redundantJointIndicies_.at(n);
        baseJoints.col(redundantJointIndex) += redundantJointActuations.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
      }

      arma::Mat<double>::fixed<3, 4> endEffectorJointsRotated = get3DRotationMatrix(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointsRelative_;
      arma::Mat<double>::fixed<3, 4> endEffectorJoints = endEffectorJointsRotated;
      endEffectorJoints.each_col() += endEffectorPosition;

      arma::Mat<double>::fixed<3, 6> passiveJoints;
      for (std::size_t n = 0; n < baseJoints.n_cols; ++n) {
        passiveJoints.col(n) = getCircleSphereIntersection(baseJoints.col(n), linkLengths_.at(0, n), baseJointsNormal_.col(n), endEffectorJoints.col(n), linkLengths_.at(1, n));
      }

      modelCharacterisation.push_back(baseJoints);
      modelCharacterisation.push_back(endEffectorJoints);
      modelCharacterisation.push_back(passiveJoints);

      return modelCharacterisation;
    }

    arma::Mat<double> ParallelKinematicMachine_4PRUS::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      std::vector<arma::Mat<double>> modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      arma::Mat<double>::fixed<3, 4> baseJointPositions = modelCharacterisation.at(0);
      arma::Mat<double>::fixed<3, 4> passiveJointPositions = modelCharacterisation.at(2);

      arma::Mat<double>::fixed<3, 4> baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<4> actuation;
      for (std::size_t n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation.at(n) = std::atan2(baseToPassiveJointPositions.at(0, n), baseToPassiveJointPositions.at(2, n));
      }

      return actuation;
    }

    double ParallelKinematicMachine_4PRUS::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      std::vector<arma::Mat<double>> modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      arma::Mat<double>::fixed<3, 4> baseJoints = modelCharacterisation.at(0);

      arma::Mat<double>::fixed<3, 4> endEffectorJoints = modelCharacterisation.at(1);
      arma::Mat<double>::fixed<3, 4> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 2);

      arma::Mat<double>::fixed<3, 4> passiveJoints = modelCharacterisation.at(2);

      arma::Mat<double>::fixed<3, 4> relativeBaseToPassiveJoints = passiveJoints - baseJoints;
      for (std::size_t n = 0; n < relativeBaseToPassiveJoints.n_cols; ++n) {
        relativeBaseToPassiveJoints.col(n) = baseJointsRotation_.slice(n) * relativeBaseToPassiveJoints.col(n);
      }

      arma::Mat<double>::fixed<3, 4> baseToEndEffectorJoints = endEffectorJoints - baseJoints;

      arma::Mat<double>::fixed<6, 4> forwardKinematic;
      forwardKinematic.rows(0, 2) = baseToEndEffectorJoints;
      for (std::size_t n = 0; n < baseToEndEffectorJoints.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJoints.col(n));
      }

      arma::Mat<double> inverseKinematic(6, 4 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % relativeBaseToPassiveJoints.row(1) - forwardKinematic.row(1) % relativeBaseToPassiveJoints.row(0);
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
        inverseKinematic.at(n, 4 + n) = arma::dot(baseToEndEffectorJoints.col(redundantJointIndex), redundantJointAngles_.col(redundantJointIndex));
      }

      return -1 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
