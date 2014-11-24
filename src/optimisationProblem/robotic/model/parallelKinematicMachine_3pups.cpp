#include <hop_bits/optimisationProblem/robotic/model/parallelKinematicMachine_3pups.hpp>

// HOP
#include <hop_bits/helper/geometry.hpp>

namespace hop {
  namespace robotic {
    ParallelKinematicMachine_3PUPS::ParallelKinematicMachine_3PUPS() noexcept
      : ParallelKinematicMachine_3PUPS(
          arma::Mat<double>::fixed<3, 3>({
            -0.025561381023353, 0.086293776138137, 0.12,
            0.087513292835791, -0.021010082747031, 0.12,
            -0.061951911812438, -0.065283693391106, 0.12}),
          arma::Mat<double>::fixed<3, 3>({
            -0.463708870031622, 0.417029254828353, -0.346410161513775,
            0.593012363818459, 0.193069033993384, -0.346410161513775,
            -0.129303493786837, -0.610098288821738, -0.346410161513775}),
          arma::Mat<double>::fixed<3, 3>({
            -0.247202519085512, 0.292029254828353, 0.086602540378444,
            0.376506012872349, 0.068069033993384, 0.086602540378444,
            -0.129303493786837, -0.360098288821738, 0.086602540378444}),
          arma::Row<double>::fixed<3>({
            0.39, 0.39, 0.39}),
          arma::Row<double>::fixed<3>({
            0.95, 0.95, 0.95})) {

    }

    ParallelKinematicMachine_3PUPS::ParallelKinematicMachine_3PUPS(
        const arma::Mat<double>::fixed<3, 3>& relativeEndEffectorJoints,
        const arma::Mat<double>::fixed<3, 3>& redundantJointStarts,
        const arma::Mat<double>::fixed<3, 3>& redundantJointEnds,
        const arma::Row<double>::fixed<3>& minimalActiveJointActuations,
        const arma::Row<double>::fixed<3>& maximalActiveJointActuations) noexcept
      : endEffectorJointsRelative_(relativeEndEffectorJoints),
        redundantJointStarts_(redundantJointStarts),
        redundantJointEnds_(redundantJointEnds),
        minimalActiveJointActuations_(minimalActiveJointActuations),
        maximalActiveJointActuations_(maximalActiveJointActuations),
        redundantJointsStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
        redundantJointIndicies_(arma::find(arma::any(redundantJointsStartToEnd_))),
        redundantJointAngles_(3, redundantJointIndicies_.n_elem) {
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        double redundantJointXAngle = std::atan2(redundantJointsStartToEnd_.at(1, n), redundantJointsStartToEnd_.at(0, n));
        double redundantJointYAngle = std::atan2(redundantJointsStartToEnd_.at(2, n), redundantJointsStartToEnd_.at(1, n));
        redundantJointAngles_.col(n) = arma::Col<double>::fixed<3>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }
    }

    std::vector<arma::Mat<double>> ParallelKinematicMachine_3PUPS::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation;

      if (arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::runtime_error("All values for the actuation of redundantion joints must be between [0, 1].");
      }

      arma::Col<double>::fixed<3> endEffector = endEffectorPose.subvec(0, 2);
      double endEffectorRollAngle = endEffectorPose.at(3);
      double endEffectorPitchAngle = endEffectorPose.at(4);
      double endEffectorYawAngle = endEffectorPose.at(5);

      arma::Mat<double>::fixed<3, 3> baseJoints = redundantJointStarts_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        std::size_t redundantJointIndex = redundantJointIndicies_.at(n);
        baseJoints.col(redundantJointIndex) += redundantJointActuations.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
      }

      arma::Mat<double>::fixed<3, 3> endEffectorJointsRotated = get3DRotationMatrix(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointsRelative_;
      arma::Mat<double>::fixed<3, 3> endEffectorJoints = endEffectorJointsRotated;
      endEffectorJoints.each_col() += endEffector;

      modelCharacterisation.push_back(baseJoints);
      modelCharacterisation.push_back(endEffectorJoints);

      return modelCharacterisation;
    }

    arma::Mat<double> ParallelKinematicMachine_3PUPS::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      arma::Mat<double>::fixed<3, 3> baseJoints = modelCharacterisation.at(0);
      arma::Mat<double>::fixed<3, 3> endEffectorJoints = modelCharacterisation.at(1);

      return arma::sqrt(arma::sum(arma::square(endEffectorJoints - baseJoints)));
    }

    double ParallelKinematicMachine_3PUPS::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      arma::Mat<double>::fixed<3, 3> baseJoints = modelCharacterisation.at(1);

      arma::Mat<double>::fixed<3, 3> endEffectorJoints = modelCharacterisation.at(1);
      arma::Mat<double>::fixed<3, 3> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      arma::Mat<double>::fixed<3, 3> baseToEndEffectorJointPositions = endEffectorJoints - baseJoints;
      arma::Col<double>::fixed<3> baseToEndEffectorJointActuations = arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));

      if (any(baseToEndEffectorJointActuations < minimalActiveJointActuations_) || any(baseToEndEffectorJointActuations > maximalActiveJointActuations_)) {
        return 0;
      }

      arma::Mat<double>::fixed<6, 3> forwardKinematic;
      forwardKinematic.rows(0, 2) = baseToEndEffectorJointPositions;
      for (std::size_t n = 0; n < baseToEndEffectorJointPositions.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJointPositions.col(n));
      }

      arma::Mat<double> inverseKinematic(6, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
        inverseKinematic.at(n, 3 + n) = arma::dot(baseToEndEffectorJointPositions.col(redundantJointIndex), redundantJointAngles_.col(redundantJointIndex));
      }

      return -1 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
