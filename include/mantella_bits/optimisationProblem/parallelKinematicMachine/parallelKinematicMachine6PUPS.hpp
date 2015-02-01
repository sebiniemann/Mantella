namespace mant {
  namespace robotic {
    class ParallelKinematicMachine6PUPS : public ParallelKinematicMachine {
      public:
        inline explicit ParallelKinematicMachine6PUPS() noexcept;
        inline explicit ParallelKinematicMachine6PUPS(
            const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints,
            const arma::Mat<double>::fixed<3, 6>& redundantJointStarts,
            const arma::Mat<double>::fixed<3, 6>& redundantJointEnds,
            const arma::Row<double>::fixed<6>& minimalActiveJointActuations,
            const arma::Row<double>::fixed<6>& maximalActiveJointActuations) noexcept;

        inline std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const override;

        inline arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const override;

        inline double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const override;

      protected:
        arma::Mat<double>::fixed<3, 6> endEffectorJointsRelative_;

        arma::Mat<double>::fixed<3, 6> redundantJointStarts_;
        arma::Mat<double>::fixed<3, 6> redundantJointEnds_;

        arma::Row<double>::fixed<6> minimalActiveJointActuations_;
        arma::Row<double>::fixed<6> maximalActiveJointActuations_;

        arma::Mat<double>::fixed<3, 6> redundantJointsStartToEnd_;
        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };

    //
    // Implementation
    //

    inline ParallelKinematicMachine6PUPS::ParallelKinematicMachine6PUPS() noexcept
      : ParallelKinematicMachine6PUPS(
          arma::Mat<double>::fixed<3, 6>({
            -0.025561381023353, 0.086293776138137, 0.12,
            0.025561381023353, 0.086293776138137, 0.12,
            0.087513292835791, -0.021010082747031, 0.12,
            0.061951911812438, -0.065283693391106, 0.12,
            -0.061951911812438, -0.065283693391106, 0.12,
            -0.087513292835791, -0.021010082747032, 0.12}),
          arma::Mat<double>::fixed<3, 6>({
            -0.463708870031622, 0.417029254828353, -0.346410161513775,
            0.463708870031622, 0.417029254828353, -0.346410161513775,
            0.593012363818459, 0.193069033993384, -0.346410161513775,
            0.129303493786838, -0.610098288821738, -0.346410161513775,
            -0.129303493786837, -0.610098288821738, -0.346410161513775,
            -0.593012363818459, 0.193069033993384, -0.346410161513775}),
          arma::Mat<double>::fixed<3, 6>({
            -0.247202519085512, 0.292029254828353, 0.086602540378444,
            0.247202519085512, 0.292029254828353, 0.086602540378444,
            0.376506012872349, 0.068069033993384, 0.086602540378444,
            0.129303493786838, -0.360098288821738, 0.086602540378444,
            -0.129303493786837, -0.360098288821738, 0.086602540378444,
            -0.376506012872349, 0.068069033993384, 0.086602540378444}),
          arma::Row<double>::fixed<6>({
            0.39, 0.39, 0.39, 0.39, 0.39, 0.39}),
          arma::Row<double>::fixed<6>({
            0.95, 0.95, 0.95, 0.95, 0.95, 0.95})) {

    }

    inline ParallelKinematicMachine6PUPS::ParallelKinematicMachine6PUPS(
        const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints,
        const arma::Mat<double>::fixed<3, 6>& redundantJointStarts,
        const arma::Mat<double>::fixed<3, 6>& redundantJointEnds,
        const arma::Row<double>::fixed<6>& minimalActiveJointActuations,
        const arma::Row<double>::fixed<6>& maximalActiveJointActuations) noexcept
      : endEffectorJointsRelative_(relativeEndEffectorJoints),
        redundantJointStarts_(redundantJointStarts),
        redundantJointEnds_(redundantJointEnds),
        minimalActiveJointActuations_(minimalActiveJointActuations),
        maximalActiveJointActuations_(maximalActiveJointActuations),
        redundantJointsStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
        redundantJointIndicies_(arma::find(arma::any(redundantJointsStartToEnd_))),
        redundantJointAngles_(3, redundantJointIndicies_.n_elem) {
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double& redundantJointXAngle = std::atan2(redundantJointsStartToEnd_.at(1, n), redundantJointsStartToEnd_.at(0, n));
        const double& redundantJointYAngle = std::atan2(redundantJointsStartToEnd_.at(2, n), redundantJointsStartToEnd_.at(1, n));
        redundantJointAngles_.col(n) = arma::Col<double>::fixed<3>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }
    }

    inline std::vector<arma::Mat<double>> ParallelKinematicMachine6PUPS::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      std::vector<arma::Mat<double>> modelCharacterisation;

      if (arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::logic_error("All values for the actuation of redundantion joints must be between [0, 1].");
      }

      const arma::Col<double>::fixed<3>& endEffectorPosition = endEffectorPose.subvec(0, 2);
      const double& endEffectorRollAngle = endEffectorPose.at(3);
      const double& endEffectorPitchAngle = endEffectorPose.at(4);
      const double& endEffectorYawAngle = endEffectorPose.at(5);

      arma::Mat<double>::fixed<3, 6> baseJoints = redundantJointStarts_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        baseJoints.col(redundantJointIndex) += redundantJointActuations.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
      }

      arma::Mat<double>::fixed<3, 6> endEffectorJoints = get3DRotationMatrix(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointsRelative_;
      endEffectorJoints.each_col() += endEffectorPosition;

      modelCharacterisation.push_back(baseJoints);
      modelCharacterisation.push_back(endEffectorJoints);

      return modelCharacterisation;
    }

    inline arma::Mat<double> ParallelKinematicMachine6PUPS::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 6>& baseJoints = modelCharacterisation.at(0);
      const arma::Mat<double>::fixed<3, 6>& endEffectorJoints = modelCharacterisation.at(1);

      return arma::sqrt(arma::sum(arma::square(endEffectorJoints - baseJoints)));
    }

    inline double ParallelKinematicMachine6PUPS::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 6>& baseJoints = modelCharacterisation.at(1);

      const arma::Mat<double>::fixed<3, 6>& endEffectorJoints = modelCharacterisation.at(1);
      arma::Mat<double>::fixed<3, 6> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 2);

      const arma::Mat<double>::fixed<3, 6>& baseToEndEffectorJointPositions = endEffectorJoints - baseJoints;
      const arma::Row<double>::fixed<6>& baseToEndEffectorJointActuations = arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));

      if (arma::any(baseToEndEffectorJointActuations < minimalActiveJointActuations_) || arma::any(baseToEndEffectorJointActuations > maximalActiveJointActuations_)) {
        return 0.0;
      }

      arma::Mat<double>::fixed<6, 6> forwardKinematic;
      forwardKinematic.rows(0, 2) = baseToEndEffectorJointPositions;
      for (std::size_t n = 0; n < baseToEndEffectorJointPositions.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJointPositions.col(n));
      }

      arma::Mat<double> inverseKinematic(6, 6 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        inverseKinematic.at(n, 6 + n) = arma::dot(baseToEndEffectorJointPositions.col(redundantJointIndex), redundantJointAngles_.col(redundantJointIndex));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
