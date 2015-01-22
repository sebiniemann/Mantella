namespace mant {
  namespace robotic {
    class ParallelKinematicMachine_6PRUS : public KinematicMachine {
      public:
        inline explicit ParallelKinematicMachine_6PRUS() noexcept;
        inline explicit ParallelKinematicMachine_6PRUS(
            const arma::Mat<double>::fixed<2, 6>& baseJointsAngles,
            const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints,
            const arma::Mat<double>::fixed<2, 6>& linkLengths,
            const arma::Mat<double>::fixed<3, 6>& redundantJointStarts,
            const arma::Mat<double>::fixed<3, 6>& redundantJointEnds) noexcept;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        ParallelKinematicMachine_6PRUS(const ParallelKinematicMachine_6PRUS&) = delete;
        ParallelKinematicMachine_6PRUS& operator=(const ParallelKinematicMachine_6PRUS&) = delete;

        inline std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        inline arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        inline double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

      protected:
        arma::Mat<double>::fixed<3, 6> endEffectorJointsRelative_;
        arma::Mat<double>::fixed<3, 6> linkLengths_;

        arma::Cube<double>::fixed<3, 3, 6> baseJointsRotation_;
        arma::Mat<double>::fixed<3, 6> baseJointsNormal_;

        arma::Mat<double>::fixed<3, 6> redundantJointStarts_;
        arma::Mat<double>::fixed<3, 6> redundantJointEnds_;
        arma::Mat<double>::fixed<3, 6> redundantJointsStartToEnd_;
        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };

    inline ParallelKinematicMachine_6PRUS::ParallelKinematicMachine_6PRUS() noexcept
      : ParallelKinematicMachine_6PRUS(
          arma::Mat<double>::fixed<2, 6>({
            0.0, 6.143558967020040,
            0.0, 0.139626340159546,
            0.0, 1.954768762233649,
            0.0, 2.234021442552742,
            0.0, 4.049163864626845,
            0.0, 4.328416544945937}),

          arma::Mat<double>::fixed<3, 6>({
            -0.027346281319362, 0.067684421383375, 0.0,
            0.027346281319362, 0.067684421383375, 0.0,
            0.072289569018135, -0.010159636370085, 0.0,
            0.044943287698773, -0.057524785013291, 0.0,
            -0.044943287698773, -0.057524785013291, 0.0,
            -0.072289569018135, -0.010159636370085, 0.0}),

          arma::Mat<double>::fixed<2, 6>({
            0.24, 0.56,
            0.24, 0.56,
            0.24, 0.56,
            0.24, 0.56,
            0.24, 0.56,
            0.24, 0.56}),

          arma::Mat<double>::fixed<3, 6>({
            -0.050659008749464, 0.360457577021932, -0.6,
            0.050659008749464, 0.360457577021932, -0.6,
            0.337494923062311, -0.136356800003392, -0.6,
            0.286835914312847, -0.224100777018540, -0.6,
            -0.286835914312847, -0.224100777018540, -0.6,
            -0.337494923062311, -0.136356800003392, -0.6}),

          arma::Mat<double>::fixed<3, 6>({
            -0.050659008749464, 0.360457577021932, 0.2,
            0.050659008749464, 0.360457577021932, 0.2,
            0.337494923062311, -0.136356800003392, 0.2,
            0.286835914312847, -0.224100777018540, 0.2,
            -0.286835914312847, -0.224100777018540, 0.2,
            -0.337494923062311, -0.136356800003392, 0.2})) {

    }

    inline ParallelKinematicMachine_6PRUS::ParallelKinematicMachine_6PRUS(
        const arma::Mat<double>::fixed<2, 6>& baseJointsAngles,
        const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints,
        const arma::Mat<double>::fixed<2, 6>& linkLengths,
        const arma::Mat<double>::fixed<3, 6>& redundantJointStarts,
        const arma::Mat<double>::fixed<3, 6>& redundantJointEnds) noexcept
      : endEffectorJointsRelative_(relativeEndEffectorJoints),
        linkLengths_(linkLengths),
        redundantJointStarts_(redundantJointStarts),
        redundantJointEnds_(redundantJointEnds),
        redundantJointsStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
        redundantJointIndicies_(arma::find(arma::any(redundantJointsStartToEnd_))),
        redundantJointAngles_(3, redundantJointIndicies_.n_elem) {
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double& redundantJointXAngle = std::atan2(redundantJointsStartToEnd_.at(1, n), redundantJointsStartToEnd_.at(0, n));
        const double& redundantJointYAngle = std::atan2(redundantJointsStartToEnd_.at(2, n), redundantJointsStartToEnd_.at(1, n));
        redundantJointAngles_.col(n) = arma::Col<double>::fixed<3>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }

      for (std::size_t n = 0; n < baseJointsAngles.n_cols; ++n) {
        baseJointsRotation_.slice(n) = get3DRotationMatrix(redundantJointAngles_.at(0, n), 0, redundantJointAngles_.at(1, n));
      }

      for (std::size_t n = 0; n < baseJointsRotation_.n_slices; ++n) {
        baseJointsNormal_.col(n) = arma::normalise(arma::cross(baseJointsRotation_.slice(n).col(0), baseJointsRotation_.slice(n).col(2)));
      }
    }

    inline std::vector<arma::Mat<double>> ParallelKinematicMachine_6PRUS::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation;

      if (arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::runtime_error("All values for the actuation of redundantion joints must be between [0, 1].");
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

      arma::Mat<double>::fixed<3, 6> passiveJoints;
      for (std::size_t n = 0; n < baseJoints.n_cols; ++n) {
        passiveJoints.col(n) = getCircleSphereIntersection(baseJoints.col(n), linkLengths_.at(0, n), baseJointsNormal_.col(n), endEffectorJoints.col(n), linkLengths_.at(1, n));
      }

      modelCharacterisation.push_back(baseJoints);
      modelCharacterisation.push_back(endEffectorJoints);
      modelCharacterisation.push_back(passiveJoints);

      return modelCharacterisation;
    }

    inline arma::Mat<double> ParallelKinematicMachine_6PRUS::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 6>& baseJointPositions = modelCharacterisation.at(0);
      const arma::Mat<double>::fixed<3, 6>& passiveJointPositions = modelCharacterisation.at(2);

      const arma::Mat<double>::fixed<3, 6>& baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<6> actuation;
      for (std::size_t n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation.at(n) = std::atan2(baseToPassiveJointPositions.at(0, n), baseToPassiveJointPositions.at(2, n));
      }

      return actuation;
    }

    inline double ParallelKinematicMachine_6PRUS::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 6>& baseJoints = modelCharacterisation.at(0);

      const arma::Mat<double>::fixed<3, 6>& endEffectorJoints = modelCharacterisation.at(1);
      arma::Mat<double>::fixed<3, 6> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 2);

      const arma::Mat<double>::fixed<3, 6>& passiveJoints = modelCharacterisation.at(2);

      arma::Mat<double>::fixed<3, 6> relativeBaseToPassiveJoints = passiveJoints - baseJoints;
      for (std::size_t n = 0; n < relativeBaseToPassiveJoints.n_cols; ++n) {
        relativeBaseToPassiveJoints.col(n) = baseJointsRotation_.slice(n) * relativeBaseToPassiveJoints.col(n);
      }

      const arma::Mat<double>::fixed<3, 6>& baseToEndEffectorJoints = endEffectorJoints - baseJoints;

      arma::Mat<double>::fixed<6, 6> forwardKinematic;
      forwardKinematic.rows(0, 2) = baseToEndEffectorJoints;
      for (std::size_t n = 0; n < baseToEndEffectorJoints.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJoints.col(n));
      }

      arma::Mat<double> inverseKinematic(6, 6 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % relativeBaseToPassiveJoints.row(1) - forwardKinematic.row(1) % relativeBaseToPassiveJoints.row(0);
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        inverseKinematic.at(n, 6 + n) = arma::dot(baseToEndEffectorJoints.col(redundantJointIndex), redundantJointAngles_.col(redundantJointIndex));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
