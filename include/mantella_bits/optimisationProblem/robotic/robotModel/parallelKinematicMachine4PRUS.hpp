namespace mant {
  namespace robotic {
    class ParallelKinematicMachine4PRUS {
      public:
        inline explicit ParallelKinematicMachine4PRUS() noexcept;

        inline arma::Mat<double>::fixed<2, 4> getLinkLengths() const noexcept;

        inline void setLinkLengths(
            const arma::Mat<double>::fixed<2, 4> linkLengths) noexcept;

        inline arma::Mat<double>::fixed<3, 4> getEndEffectorJointPositions() const noexcept;

        inline void setEndEffectorJointPositions(
            const arma::Mat<double>::fixed<3, 4> endEffectorJointPositions) noexcept;

        inline arma::Mat<double>::fixed<3, 4> getRedundantJointStartPositions() const noexcept;

        inline void setRedundantJointStartPositions(
            const arma::Mat<double>::fixed<3, 4> redundantJointStartPositions) noexcept;

        inline arma::Mat<double>::fixed<3, 4> getRedundantJointEndPositions() const noexcept;

        inline void setRedundantJointEndPositions(
            const arma::Mat<double>::fixed<3, 4> redundantJointEndPositions) noexcept;

        inline arma::Mat<double>::fixed<2, 4> getBaseJointAngles() const noexcept;

        inline void setBaseJointAngles(
            const arma::Mat<double>::fixed<2, 4> baseJointAngles) noexcept;

        inline arma::Cube<double>::fixed<3, 4, 3> getModel(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

        inline arma::Row<double>::fixed<4> getActuation(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

        inline arma::Col<double>::fixed<6> getEndEffectorPose(
            const arma::Row<double>::fixed<4>& actuations,
            const arma::Row<double>::fixed<3>& endEffectorRotation,
            const arma::Row<double>& redundantJointActuations) const;

        inline double getEndEffectorPoseError(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

      protected:
        arma::Mat<double>::fixed<2, 4> linkLengths_;

        arma::Mat<double>::fixed<3, 4> endEffectorJointPositions_;

        arma::Mat<double>::fixed<3, 4> redundantJointStartPositions_;
        arma::Mat<double>::fixed<3, 4> redundantJointEndPositions_;

        arma::Mat<double>::fixed<2, 4> baseJointAngles_;

        arma::Mat<double>::fixed<3, 4> redundantJointStartToEndPositions_;

        arma::Cube<double>::fixed<3, 3, 4> baseJointRotations_;
        arma::Mat<double>::fixed<3, 4> baseJointNormals_;

        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };

    //
    // Implementation
    //

    inline ParallelKinematicMachine4PRUS::ParallelKinematicMachine4PRUS() noexcept {
      setLinkLengths({
        0.24, 0.56,
        0.24, 0.56,
        0.24, 0.56,
        0.24, 0.56});

      setEndEffectorJointPositions({
        0.073, 0.0, 0.0,
        0.0, 0.073, 0.0,
        -0.073, 0.0, 0.0,
        0.0, -0.073, 0.0});

      setRedundantJointStartPositions({
        0.364, 0.0, -0.6,
        0.0, 0.364, -0.6,
        -0.364, 0.0, -0.6,
        0.0, -0.364, -0.6});

      setRedundantJointEndPositions({
        0.364, 0.0, 0.2,
        0.0, 0.364, 0.2,
        -0.364, 0.0, 0.2,
        0.0, -0.364, 0.2});

      setBaseJointAngles({
        0.0, 0.0,
        0.0, 1.570796326794897,
        0.0, 3.141592653589793,
        0.0, 4.712388980384690});

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

    inline arma::Mat<double>::fixed<2, 4> ParallelKinematicMachine4PRUS::getLinkLengths() const noexcept {
      return linkLengths_;
    }

    inline void ParallelKinematicMachine4PRUS::setLinkLengths(
        const arma::Mat<double>::fixed<2, 4> linkLengths) noexcept {
      linkLengths_ = linkLengths;
    }

    inline arma::Mat<double>::fixed<3, 4> ParallelKinematicMachine4PRUS::getEndEffectorJointPositions() const noexcept {
      return endEffectorJointPositions_;
    }

    inline void ParallelKinematicMachine4PRUS::setEndEffectorJointPositions(
        const arma::Mat<double>::fixed<3, 4> endEffectorJointPositions) noexcept {
      endEffectorJointPositions_ = endEffectorJointPositions;
    }

    inline arma::Mat<double>::fixed<3, 4> ParallelKinematicMachine4PRUS::getRedundantJointStartPositions() const noexcept {
      return redundantJointStartPositions_;
    }

    inline void ParallelKinematicMachine4PRUS::setRedundantJointStartPositions(
        const arma::Mat<double>::fixed<3, 4> redundantJointStartPositions) noexcept {
      redundantJointStartPositions_ = redundantJointStartPositions;
    }

    inline arma::Mat<double>::fixed<3, 4> ParallelKinematicMachine4PRUS::getRedundantJointEndPositions() const noexcept {
      return redundantJointEndPositions_;
    }

    inline void ParallelKinematicMachine4PRUS::setRedundantJointEndPositions(
        const arma::Mat<double>::fixed<3, 4> redundantJointEndPositions) noexcept {
      redundantJointEndPositions_ = redundantJointEndPositions;
    }

    inline arma::Mat<double>::fixed<2, 4> ParallelKinematicMachine4PRUS::getBaseJointAngles() const noexcept {
      return baseJointAngles_;
    }

    inline void ParallelKinematicMachine4PRUS::setBaseJointAngles(
        const arma::Mat<double>::fixed<2, 4> baseJointAngles) noexcept {
      baseJointAngles_ = baseJointAngles;
    }

    inline arma::Cube<double>::fixed<3, 4, 3> ParallelKinematicMachine4PRUS::getModel(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the actuation of redundantion joints must be between [0, 1].");
      verify(redundantJointActuations.n_elem == redundantJointIndicies_.n_elem, "The number of redundant actuations must be equal to the number of redundant joints.");

      arma::Cube<double>::fixed<3, 4, 3> model;

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

    inline arma::Row<double>::fixed<4> ParallelKinematicMachine4PRUS::getActuation(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<3, 4, 3>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 4>& baseJointPositions = model.slice(0);
      const arma::Mat<double>::fixed<3, 4>& passiveJointPositions = model.slice(1);

      const arma::Mat<double>::fixed<3, 4>& baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<4> actuation;
      for (std::size_t n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation(n) = std::atan2(baseToPassiveJointPositions(0, n), baseToPassiveJointPositions(2, n));
      }

      return actuation;
    }

    inline arma::Col<double>::fixed<6> ParallelKinematicMachine4PRUS::getEndEffectorPose(
        const arma::Row<double>::fixed<4>& actuations,
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

    inline double ParallelKinematicMachine4PRUS::getEndEffectorPoseError(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<3, 4, 3>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 4>& baseJoints = model.slice(0);

      const arma::Mat<double>::fixed<3, 4>& endEffectorJoints = model.slice(2);
      arma::Mat<double>::fixed<3, 4> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 2);

      const arma::Mat<double>::fixed<3, 4>& passiveJoints = model.slice(1);

      arma::Mat<double>::fixed<3, 4> relativeBaseToPassiveJoints = passiveJoints - baseJoints;
      for (std::size_t n = 0; n < relativeBaseToPassiveJoints.n_cols; ++n) {
        relativeBaseToPassiveJoints.col(n) = baseJointRotations_.slice(n) * relativeBaseToPassiveJoints.col(n);
      }

      const arma::Mat<double>::fixed<3, 4>& baseToEndEffectorJoints = endEffectorJoints - baseJoints;

      arma::Mat<double>::fixed<6, 4> forwardKinematic;
      forwardKinematic.head_rows(3) = baseToEndEffectorJoints;
      for (std::size_t n = 0; n < baseToEndEffectorJoints.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJoints.col(n));
      }

      arma::Mat<double> inverseKinematic(6, 4 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % relativeBaseToPassiveJoints.row(1) - forwardKinematic.row(1) % relativeBaseToPassiveJoints.row(0);
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        inverseKinematic(n, 4 + n) = arma::dot(baseToEndEffectorJoints.col(redundantJointIndex), redundantJointAngles_.col(redundantJointIndex));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
