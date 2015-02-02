namespace mant {
  namespace robotic {
    class ParallelKinematicMachine3PRUS {
      public:
        inline explicit ParallelKinematicMachine3PRUS() noexcept;
        inline std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept;

        inline arma::Mat<double>::fixed<2, 3> getLinkLengths() const noexcept;

        inline arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept;
        inline void setLinkLengths(
            const arma::Mat<double>::fixed<2, 3>& linkLengths) noexcept;

        inline double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept;
        inline arma::Mat<double>::fixed<3, 3> getEndEffectorJointPositions() const noexcept;

        inline void setEndEffectorJointPositions(
            const arma::Mat<double>::fixed<3, 3>& endEffectorJointPositions) noexcept;

        inline arma::Mat<double>::fixed<3, 3> getRedundantJointStartPositions() const noexcept;

        inline void setRedundantJointStartPositions(
            const arma::Mat<double>::fixed<3, 3>& redundantJointStartPositions) noexcept;

        inline arma::Mat<double>::fixed<3, 3> getRedundantJointEndPositions() const noexcept;

        inline void setRedundantJointEndPositions(
            const arma::Mat<double>::fixed<3, 3>& redundantJointEndPositions) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getBaseJointAngles() const noexcept;

        inline void setBaseJointAngles(
            const arma::Mat<double>::fixed<2, 3>& baseJointAngles) noexcept;

      protected:
        arma::Mat<double>::fixed<3, 3> endEffectorJointsRelative_;
        arma::Mat<double>::fixed<3, 3> linkLengths_;

        arma::Cube<double>::fixed<3, 3, 3> baseJointsRotation_;
        arma::Mat<double>::fixed<3, 3> baseJointsNormal_;

        arma::Mat<double>::fixed<3, 3> redundantJointStarts_;
        arma::Mat<double>::fixed<3, 3> redundantJointEnds_;
        arma::Mat<double>::fixed<3, 3> redundantJointsStartToEnd_;
        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };

    //
    // Implementation
    //

    inline ParallelKinematicMachine3PRUS::ParallelKinematicMachine3PRUS() noexcept {
      setLinkLengths({
        0.24, 0.56,
        0.24, 0.56,
        0.24, 0.56});

      setEndEffectorJointPositions({
        -0.027346281319362, 0.067684421383375, 0.0,
        0.072289569018135, -0.010159636370085, 0.0,
        -0.044943287698773, -0.057524785013291, 0.0});

      setRedundantJointStartPositions({
        -0.050659008749464, 0.360457577021932, -0.6,
        0.337494923062311, -0.136356800003392, -0.6,
        -0.286835914312847, -0.224100777018540, -0.6});

      setRedundantJointEndPositions({
        -0.050659008749464, 0.360457577021932, 0.2,
        0.337494923062311, -0.136356800003392, 0.2,
        -0.286835914312847, -0.224100777018540, 0.2});

      setBaseJointAngles({
        0.0, 6.143558967020040,
        0.0, 1.954768762233649,
        0.0, 4.049163864626845});


      redundantJointStartToEndPositions_ = redundantJointEndPositions_ - redundantJointStartPositions_;
      redundantJointIndicies_ = arma::find(arma::any(redundantJointStartToEndPositions_));

      redundantJointAngles_.set_size(3, redundantJointIndicies_.n_elem);

      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double& redundantJointXAngle = std::atan2(redundantJointStartToEndPositions_.at(1, n), redundantJointStartToEndPositions_.at(0, n));
        const double& redundantJointYAngle = std::atan2(redundantJointStartToEndPositions_.at(2, n), redundantJointStartToEndPositions_.at(1, n));
        redundantJointAngles_.col(n) = arma::Col<double>::fixed<3>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }

      for (std::size_t n = 0; n < baseJointAngles_.n_cols; ++n) {
        baseJointRotations_.slice(n) = get3DRotationMatrix(redundantJointAngles_.at(0, n), 0, redundantJointAngles_.at(1, n));
      }

      for (std::size_t n = 0; n < baseJointRotations_.n_slices; ++n) {
        baseJointNormals_.col(n) = arma::normalise(arma::cross(baseJointRotations_.slice(n).col(0), baseJointRotations_.slice(n).col(2)));
      }
    }

    inline std::vector<arma::Mat<double>> ParallelKinematicMachine3PRUS::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation;
    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRUS::getLinkLengths() const noexcept {
      return linkLengths_;
    }

    inline void ParallelKinematicMachine3PRUS::setLinkLengths(
        const arma::Mat<double>::fixed<2, 3>& linkLengths) noexcept {
      linkLengths_ = linkLengths;
    }

    inline arma::Mat<double>::fixed<3, 3> ParallelKinematicMachine3PRUS::getEndEffectorJointPositions() const noexcept {
      return endEffectorJointPositions_;
    }

    inline void ParallelKinematicMachine3PRUS::setEndEffectorJointPositions(
        const arma::Mat<double>::fixed<3, 3>& endEffectorJointPositions) noexcept {
      endEffectorJointPositions_ = endEffectorJointPositions;
    }

    inline arma::Mat<double>::fixed<3, 3> ParallelKinematicMachine3PRUS::getRedundantJointStartPositions() const noexcept {
      return redundantJointStartPositions_;
    }

    inline void ParallelKinematicMachine3PRUS::setRedundantJointStartPositions(
        const arma::Mat<double>::fixed<3, 3>& redundantJointStartPositions) noexcept {
      redundantJointStartPositions_ = redundantJointStartPositions;
    }

    inline arma::Mat<double>::fixed<3, 3> ParallelKinematicMachine3PRUS::getRedundantJointEndPositions() const noexcept {
      return redundantJointEndPositions_;
    }

    inline void ParallelKinematicMachine3PRUS::setRedundantJointEndPositions(
        const arma::Mat<double>::fixed<3, 3>& redundantJointEndPositions) noexcept {
      redundantJointEndPositions_ = redundantJointEndPositions;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRUS::getBaseJointAngles() const noexcept {
      return baseJointAngles_;
    }

    inline void ParallelKinematicMachine3PRUS::setBaseJointAngles(
        const arma::Mat<double>::fixed<2, 3>& baseJointAngles) noexcept {
      baseJointAngles_ = baseJointAngles;
    }

      if (arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::logic_error("All values for the actuation of redundantion joints must be between [0, 1].");
      }

      const arma::Col<double>::fixed<3>& endEffectorPosition = endEffectorPose.subvec(0, 2);
      const double& endEffectorRollAngle = endEffectorPose.at(3);
      const double& endEffectorPitchAngle = endEffectorPose.at(4);
      const double& endEffectorYawAngle = endEffectorPose.at(5);

      arma::Mat<double>::fixed<3, 3> baseJoints = redundantJointStarts_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        baseJoints.col(redundantJointIndex) += redundantJointActuations.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
      }

      arma::Mat<double>::fixed<3, 3> endEffectorJoints = get3DRotationMatrix(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointsRelative_;
      endEffectorJoints.each_col() += endEffectorPosition;

      arma::Mat<double>::fixed<3, 3> passiveJoints;
      for (std::size_t n = 0; n < baseJoints.n_cols; ++n) {
        passiveJoints.col(n) = getCircleSphereIntersection(baseJoints.col(n), linkLengths_.at(0, n), baseJointsNormal_.col(n), endEffectorJoints.col(n), linkLengths_.at(1, n));
      }

      modelCharacterisation.push_back(baseJoints);
      modelCharacterisation.push_back(endEffectorJoints);
      modelCharacterisation.push_back(passiveJoints);

      return modelCharacterisation;
    }

    inline arma::Mat<double> ParallelKinematicMachine3PRUS::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 3>& baseJointPositions = modelCharacterisation.at(0);
      const arma::Mat<double>::fixed<3, 3>& passiveJointPositions = modelCharacterisation.at(2);

      const arma::Mat<double>::fixed<3, 3>& baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<3> actuation;
      for (std::size_t n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation.at(n) = std::atan2(baseToPassiveJointPositions.at(0, n), baseToPassiveJointPositions.at(2, n));
      }

      return actuation;
    }

    inline double ParallelKinematicMachine3PRUS::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 3>& baseJoints = modelCharacterisation.at(0);

      const arma::Mat<double>::fixed<3, 3>& endEffectorJoints = modelCharacterisation.at(1);
      arma::Mat<double>::fixed<3, 3> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 2);

      const arma::Mat<double>::fixed<3, 3>& passiveJoints = modelCharacterisation.at(2);

      arma::Mat<double>::fixed<3, 3> relativeBaseToPassiveJoints = passiveJoints - baseJoints;
      for (std::size_t n = 0; n < relativeBaseToPassiveJoints.n_cols; ++n) {
        relativeBaseToPassiveJoints.col(n) = baseJointsRotation_.slice(n) * relativeBaseToPassiveJoints.col(n);
      }

      const arma::Mat<double>::fixed<3, 3>& baseToEndEffectorJoints = endEffectorJoints - baseJoints;

      arma::Mat<double>::fixed<6, 3> forwardKinematic;
      forwardKinematic.rows(0, 2) = baseToEndEffectorJoints;
      for (std::size_t n = 0; n < baseToEndEffectorJoints.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJoints.col(n));
      }

      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % relativeBaseToPassiveJoints.row(1) - forwardKinematic.row(1) % relativeBaseToPassiveJoints.row(0);
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        inverseKinematic.at(n, 3 + n) = arma::dot(baseToEndEffectorJoints.col(redundantJointIndex), redundantJointAngles_.col(redundantJointIndex));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
