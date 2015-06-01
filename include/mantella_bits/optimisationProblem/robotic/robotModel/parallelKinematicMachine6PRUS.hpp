namespace mant {
  namespace robotic {
    class ParallelKinematicMachine6PRUS {
      public:
        inline explicit ParallelKinematicMachine6PRUS() noexcept;

        inline arma::Mat<double>::fixed<2, 6> getLinkLengths() const noexcept;

        inline void setLinkLengths(
            const arma::Mat<double>::fixed<2, 6> linkLengths) noexcept;

        inline arma::Mat<double>::fixed<3, 6> getEndEffectorJointPositions() const noexcept;

        inline void setEndEffectorJointPositions(
            const arma::Mat<double>::fixed<3, 6> endEffectorJointPositions) noexcept;

        inline arma::Mat<double>::fixed<3, 6> getRedundantJointStartPositions() const noexcept;

        inline void setRedundantJointStartPositions(
            const arma::Mat<double>::fixed<3, 6> redundantJointStartPositions) noexcept;

        inline arma::Mat<double>::fixed<3, 6> getRedundantJointEndPositions() const noexcept;

        inline void setRedundantJointEndPositions(
            const arma::Mat<double>::fixed<3, 6> redundantJointEndPositions) noexcept;

        inline arma::Mat<double>::fixed<2, 6> getBaseJointAngles() const noexcept;

        inline void setBaseJointAngles(
            const arma::Mat<double>::fixed<2, 6> baseJointAngles) noexcept;

        inline arma::Cube<double>::fixed<3, 6, 3> getModel(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

        inline arma::Row<double>::fixed<6> getActuation(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

        inline arma::Col<double>::fixed<6> getEndEffectorPose(
            const arma::Row<double>::fixed<6>& actuations,
            const arma::Row<double>::fixed<3>& endEffectorRotation,
            const arma::Row<double>& redundantJointActuations) const;

        inline double getEndEffectorPoseError(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

      protected:
        arma::Mat<double>::fixed<2, 6> linkLengths_;

        arma::Mat<double>::fixed<3, 6> endEffectorJointPositions_;

        arma::Mat<double>::fixed<3, 6> redundantJointStartPositions_;
        arma::Mat<double>::fixed<3, 6> redundantJointEndPositions_;

        arma::Mat<double>::fixed<2, 6> baseJointAngles_;

        arma::Mat<double>::fixed<3, 6> redundantJointStartToEndPositions_;

        arma::Cube<double>::fixed<3, 3, 6> baseJointRotations_;
        arma::Mat<double>::fixed<3, 6> baseJointNormals_;

        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };

    //
    // Implementation
    //

    inline ParallelKinematicMachine6PRUS::ParallelKinematicMachine6PRUS() noexcept {
      setLinkLengths({
        0.24, 0.56,
        0.24, 0.56,
        0.24, 0.56,
        0.24, 0.56,
        0.24, 0.56,
        0.24, 0.56});

      setEndEffectorJointPositions({
        -0.027346281319362, 0.067684421383375, 0.0,
        0.027346281319362, 0.067684421383375, 0.0,
        0.072289569018135, -0.010159636370085, 0.0,
        0.044943287698773, -0.057524785013291, 0.0,
        -0.044943287698773, -0.057524785013291, 0.0,
        -0.072289569018135, -0.010159636370085, 0.0});

      setRedundantJointStartPositions({
        -0.050659008749464, 0.360457577021932, -0.6,
        0.050659008749464, 0.360457577021932, -0.6,
        0.337494923062311, -0.136356800003392, -0.6,
        0.286835914312847, -0.224100777018540, -0.6,
        -0.286835914312847, -0.224100777018540, -0.6,
        -0.337494923062311, -0.136356800003392, -0.6});

      setRedundantJointEndPositions({
        -0.050659008749464, 0.360457577021932, 0.2,
        0.050659008749464, 0.360457577021932, 0.2,
        0.337494923062311, -0.136356800003392, 0.2,
        0.286835914312847, -0.224100777018540, 0.2,
        -0.286835914312847, -0.224100777018540, 0.2,
        -0.337494923062311, -0.136356800003392, 0.2});

      setBaseJointAngles({
        0.0, 6.143558967020040,
        0.0, 0.139626340159546,
        0.0, 1.954768762233649,
        0.0, 2.234021442552742,
        0.0, 4.049163864626845,
        0.0, 4.328416544945937});

      redundantJointStartToEndPositions_ = redundantJointEndPositions_ - redundantJointStartPositions_;
      redundantJointIndicies_ = arma::find(arma::any(redundantJointStartToEndPositions_));

      redundantJointAngles_.set_size(3, redundantJointIndicies_.n_elem);

      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double& redundantJointXAngle = std::atan2(redundantJointStartToEndPositions_(1, n), redundantJointStartToEndPositions_(0, n));
        const double& redundantJointYAngle = std::atan2(redundantJointStartToEndPositions_(2, n), redundantJointStartToEndPositions_(1, n));
        redundantJointAngles_.col(n) = arma::Col<double>::fixed<3>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }

      for (std::size_t n = 0; n < baseJointAngles_.n_cols; ++n) {
        baseJointRotations_.slice(n) = get3DRotation(redundantJointAngles_(0, n), 0, redundantJointAngles_(1, n));
      }

      for (std::size_t n = 0; n < baseJointRotations_.n_slices; ++n) {
        baseJointNormals_.col(n) = arma::normalise(arma::cross(baseJointRotations_.slice(n).col(0), baseJointRotations_.slice(n).col(2)));
      }
    }

    inline arma::Mat<double>::fixed<2, 6> ParallelKinematicMachine6PRUS::getLinkLengths() const noexcept {
      return linkLengths_;
    }

    inline void ParallelKinematicMachine6PRUS::setLinkLengths(
        const arma::Mat<double>::fixed<2, 6> linkLengths) noexcept {
      linkLengths_ = linkLengths;
    }

    inline arma::Mat<double>::fixed<3, 6> ParallelKinematicMachine6PRUS::getEndEffectorJointPositions() const noexcept {
      return endEffectorJointPositions_;
    }

    inline void ParallelKinematicMachine6PRUS::setEndEffectorJointPositions(
        const arma::Mat<double>::fixed<3, 6> endEffectorJointPositions) noexcept {
      endEffectorJointPositions_ = endEffectorJointPositions;
    }

    inline arma::Mat<double>::fixed<3, 6> ParallelKinematicMachine6PRUS::getRedundantJointStartPositions() const noexcept {
      return redundantJointStartPositions_;
    }

    inline void ParallelKinematicMachine6PRUS::setRedundantJointStartPositions(
        const arma::Mat<double>::fixed<3, 6> redundantJointStartPositions) noexcept {
      redundantJointStartPositions_ = redundantJointStartPositions;
    }

    inline arma::Mat<double>::fixed<3, 6> ParallelKinematicMachine6PRUS::getRedundantJointEndPositions() const noexcept {
      return redundantJointEndPositions_;
    }

    inline void ParallelKinematicMachine6PRUS::setRedundantJointEndPositions(
        const arma::Mat<double>::fixed<3, 6> redundantJointEndPositions) noexcept {
      redundantJointEndPositions_ = redundantJointEndPositions;
    }

    inline arma::Mat<double>::fixed<2, 6> ParallelKinematicMachine6PRUS::getBaseJointAngles() const noexcept {
      return baseJointAngles_;
    }

    inline void ParallelKinematicMachine6PRUS::setBaseJointAngles(
        const arma::Mat<double>::fixed<2, 6> baseJointAngles) noexcept {
      baseJointAngles_ = baseJointAngles;
    }

    inline arma::Cube<double>::fixed<3, 6, 3> ParallelKinematicMachine6PRUS::getModel(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the actuation of redundantion joints must be between [0, 1].");
      verify(redundantJointActuations.n_elem == redundantJointIndicies_.n_elem, "The number of redundant actuations must be equal to the number of redundant joints.");

      arma::Cube<double>::fixed<3, 6, 3> model;

      const arma::Col<double>::fixed<3>& endEffectorPosition = endEffectorPose.subvec(0, 2);
      const double& endEffectorRollAngle = endEffectorPose(3);
      const double& endEffectorPitchAngle = endEffectorPose(4);
      const double& endEffectorYawAngle = endEffectorPose(5);

      model.slice(0) = redundantJointStartPositions_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        model.slice(0).col(redundantJointIndex) += redundantJointActuations(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      model.slice(2) = get3DRotation(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointPositions_;
      model.slice(2).each_col() += endEffectorPosition;

      for (std::size_t n = 0; n < model.slice(0).n_cols; ++n) {
        model.slice(1).col(n) = getCircleSphereIntersection(model.slice(0).col(n), linkLengths_(0, n), baseJointNormals_.col(n), model.slice(2).col(n), linkLengths_(1, n));
      }

      return model;
    }

    inline arma::Row<double>::fixed<6> ParallelKinematicMachine6PRUS::getActuation(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<3, 6, 3>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 6>& baseJointPositions = model.slice(0);
      const arma::Mat<double>::fixed<3, 6>& passiveJointPositions = model.slice(1);

      const arma::Mat<double>::fixed<3, 6>& baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<6> actuation;
      for (std::size_t n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation(n) = std::atan2(baseToPassiveJointPositions(0, n), baseToPassiveJointPositions(2, n));
      }

      return actuation;
    }

    inline arma::Col<double>::fixed<6> ParallelKinematicMachine6PRUS::getEndEffectorPose(
        const arma::Row<double>::fixed<6>& actuations,
        const arma::Row<double>::fixed<3>& endEffectorRotation,
        const arma::Row<double>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the actuation of redundantion joints must be between [0, 1].");
      verify(redundantJointActuations.n_elem == redundantJointIndicies_.n_elem, "The number of redundant actuations must be equal to the number of redundant joints.");

      const double& endEffectorRollAngle = endEffectorRotation(0);
      const double& endEffectorPitchAngle = endEffectorRotation(1);
      const double& endEffectorYawAngle = endEffectorRotation(2);

      arma::Mat<double> passiveJointPositions = redundantJointStartPositions_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        passiveJointPositions.col(redundantJointIndex) += redundantJointActuations(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      passiveJointPositions -= get3DRotation(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointPositions_;
      for(std::size_t n = 0; n < passiveJointPositions.n_cols; ++n) {
        passiveJointPositions.col(n) += get3DRotation(actuations(0), baseJointAngles_(0, n), baseJointAngles_(1, n)) * arma::Col<double>::fixed<3>({0, 0, linkLengths_(0, n)});
      }

      return arma::join_cols(getTriangulation(passiveJointPositions.col(0), linkLengths_(1, 0), passiveJointPositions.col(1), linkLengths_(1, 1), passiveJointPositions.col(2), linkLengths_(1, 2)), endEffectorRotation);
    }

    inline double ParallelKinematicMachine6PRUS::getEndEffectorPoseError(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<3, 6, 3>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 6>& baseJoints = model.slice(0);

      const arma::Mat<double>::fixed<3, 6>& endEffectorJoints = model.slice(2);
      arma::Mat<double>::fixed<3, 6> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 2);

      const arma::Mat<double>::fixed<3, 6>& passiveJoints = model.slice(1);

      arma::Mat<double>::fixed<3, 6> relativeBaseToPassiveJoints = passiveJoints - baseJoints;
      for (std::size_t n = 0; n < relativeBaseToPassiveJoints.n_cols; ++n) {
        relativeBaseToPassiveJoints.col(n) = baseJointRotations_.slice(n) * relativeBaseToPassiveJoints.col(n);
      }

      const arma::Mat<double>::fixed<3, 6>& baseToEndEffectorJoints = endEffectorJoints - baseJoints;

      arma::Mat<double>::fixed<6, 6> forwardKinematic;
      forwardKinematic.head_rows(3) = baseToEndEffectorJoints;
      for (std::size_t n = 0; n < baseToEndEffectorJoints.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJoints.col(n));
      }

      arma::Mat<double> inverseKinematic(6, 6 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % relativeBaseToPassiveJoints.row(1) - forwardKinematic.row(1) % relativeBaseToPassiveJoints.row(0);
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        inverseKinematic(n, 6 + n) = arma::dot(baseToEndEffectorJoints.col(redundantJointIndex), redundantJointAngles_.col(redundantJointIndex));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
