#include <hop_bits/optimisationProblem/robotic/model/parallelKinematicMachine_3prpr.hpp>

// HOP
#include <hop_bits/helper/geometry.hpp>

namespace hop {
  namespace robotic {
    ParallelKinematicMachine_3PRPR::ParallelKinematicMachine_3PRPR() noexcept
      : ParallelKinematicMachine_3PRPR(
          arma::Mat<double>::fixed<2, 3>({
            -0.000066580445834, 0.106954081945581,
            -0.092751709777083, -0.053477040972790,
            0.092818290222917, -0.053477040972790}),
          arma::Mat<double>::fixed<2, 3>({
            0.1, 1.0392,
            0.0, 0.8,
            1.2, 0.8}),
          arma::Mat<double>::fixed<2, 3>({
            1.1, 1.0392,
            0.0, -0.2,
            1.2, -0.2}),
          arma::Row<double>::fixed<3>({
            0.1, 0.1, 0.1}),
          arma::Row<double>::fixed<3>({
            1.2, 1.2, 1.2})) {

    }

    ParallelKinematicMachine_3PRPR::ParallelKinematicMachine_3PRPR(
        const arma::Mat<double>::fixed<2, 3>& endEffectorJointRelativePositions,
        const arma::Mat<double>::fixed<2, 3>& redundantJointStarts,
        const arma::Mat<double>::fixed<2, 3>& redundantJointEnds,
        const arma::Row<double>::fixed<3>& minimalActiveJointActuations,
        const arma::Row<double>::fixed<3>& maximalActiveJointActuations) noexcept
      : endEffectorJointRelativePositions_(endEffectorJointRelativePositions),
        redundantJointStartPositions_(redundantJointStarts),
        redundantJointEndPositions_(redundantJointEnds),
        minimalActiveJointActuations_(minimalActiveJointActuations),
        maximalActiveJointActuations_(maximalActiveJointActuations),
        redundantJointStartToEndPositions_(redundantJointEndPositions_ - redundantJointStartPositions_),
        redundantJointIndicies_(arma::find(arma::any(redundantJointStartToEndPositions_))),
        redundantJointAngleSines_(redundantJointIndicies_.n_elem),
        redundantJointAngleCosines_(redundantJointIndicies_.n_elem) {
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        double redundantJointAngle = std::atan2(redundantJointStartToEndPositions_.at(1, n), redundantJointStartToEndPositions_.at(0, n));
        redundantJointAngleSines_.at(n) = std::sin(redundantJointAngle);
        redundantJointAngleCosines_.at(n) = std::cos(redundantJointAngle);
      }
    }

    std::vector<arma::Mat<double>> ParallelKinematicMachine_3PRPR::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation;

      if (arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::runtime_error("All values for the actuation of redundantion joints must be between [0, 1].");
      }

      arma::Col<double>::fixed<2> endEffectorPosition = endEffectorPose.subvec(0, 1);
      double endEffectorAngle = endEffectorPose.at(2);

      arma::Mat<double>::fixed<2, 3> baseJointPositions = redundantJointStartPositions_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        std::size_t redundantJointIndex = redundantJointIndicies_.at(n);
        baseJointPositions.col(redundantJointIndex) += redundantJointActuations.at(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      arma::Mat<double>::fixed<2, 3> endEffectorJointRelativePositionsRotated = get2DRotationMatrix(endEffectorAngle) * endEffectorJointRelativePositions_;
      arma::Mat<double>::fixed<2, 3> endEffectorJointPositions = endEffectorJointRelativePositionsRotated;
      endEffectorJointPositions.each_col() += endEffectorPosition;

      modelCharacterisation.push_back(baseJointPositions);
      modelCharacterisation.push_back(endEffectorJointPositions);

      return modelCharacterisation;
    }

    arma::Mat<double> ParallelKinematicMachine_3PRPR::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      arma::Mat<double>::fixed<2, 3> baseJoints = modelCharacterisation.at(0);
      arma::Mat<double>::fixed<2, 3> endEffectorJoints = modelCharacterisation.at(1);

      return arma::sqrt(arma::sum(arma::square(endEffectorJoints - baseJoints)));
    }

    double ParallelKinematicMachine_3PRPR::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      arma::Mat<double>::fixed<2, 3> baseJointPositions = modelCharacterisation.at(1);

      arma::Mat<double>::fixed<2, 3> endEffectorJointPositions = modelCharacterisation.at(1);
      arma::Mat<double>::fixed<2, 3> endEffectorJointRelativePositionsRotated = endEffectorJointPositions;
      endEffectorJointRelativePositionsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      arma::Mat<double>::fixed<2, 3> baseToEndEffectorJointPositions = endEffectorJointPositions - baseJointPositions;
      arma::Col<double>::fixed<3> baseToEndEffectorJointActuations = arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));

      if (any(baseToEndEffectorJointActuations < minimalActiveJointActuations_) || any(baseToEndEffectorJointActuations > maximalActiveJointActuations_)) {
        return 0;
      }

      arma::Mat<double>::fixed<3, 3> forwardKinematic;
      forwardKinematic.rows(0, 1) = baseToEndEffectorJointPositions;
      forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointRelativePositionsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointRelativePositionsRotated.row(0);

      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = -baseToEndEffectorJointActuations;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        arma::uword redundantJointIndex = redundantJointIndicies_.at(n);
        inverseKinematic.at(n, 3 + n) = forwardKinematic.at(redundantJointIndex, 0) * redundantJointAngleCosines_.at(n) + forwardKinematic.at(redundantJointIndex, 1) * redundantJointAngleSines_.at(n);
      }

      return -1 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
