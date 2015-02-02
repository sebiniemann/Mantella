namespace mant {
  namespace robotic {
    class ParallelKinematicMachine3PRRR {
      public:
        inline explicit ParallelKinematicMachine3PRRR() noexcept;

        inline arma::Mat<double>::fixed<2, 3> getLinkLengths() const noexcept;

        inline std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept;
        inline void setLinkLengths(
            const arma::Mat<double>::fixed<2, 3>& linkLengths) noexcept;

        inline arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept;
        inline arma::Col<double>::fixed<2, 3> getEndEffectorJointPositions() const noexcept;

        inline double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept;
        inline void setEndEffectorJointPositions(
            const arma::Col<double>::fixed<2, 3>& endEffectorJointPositions) noexcept;

        arma::Mat<double>::fixed<2, 3> endEffectorJointsRelative_;
        inline arma::Mat<double>::fixed<2, 3> getRedundantJointPositionStarts() const noexcept;

        inline void setRedundantJointPositionStarts(
            const arma::Mat<double>::fixed<2, 3>& redundantJointPositionStarts) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getRedundantJointPositionEnds() const noexcept;

        inline void setRedundantJointPositionEnds(
            const arma::Mat<double>::fixed<2, 3>& redundantJointPositionEnds) noexcept;

        inline std::vector<arma::Mat<double>::fixed<2, 3>> getModel(
            const arma::Col<double>::fixed<3>& endEffectorPose,
            const arma::Col<double>& redundantJointActuations) const;

        inline arma::Col<double>::fixed<3> getActuation(
            const arma::Col<double>::fixed<3>& endEffectorPose,
            const arma::Col<double>& redundantJointActuations) const;

        inline arma::Col<double>::fixed<3> getEndEffectorPose(
            const arma::Col<double>::fixed<3>& actuations,
            const arma::Col<double>& redundantJointActuations) const;

        inline double getEndEffectorPoseAccuracy(
            const arma::Col<double>::fixed<3>& endEffectorPose,
            const arma::Col<double>& redundantJointActuations) const;

        arma::Mat<double>::fixed<2, 3> endEffectorJointPositions_;
        arma::Mat<double>::fixed<2, 3> linkLengths_;

        arma::Mat<double>::fixed<2, 3> redundantJointStarts_;
        arma::Mat<double>::fixed<2, 3> redundantJointEnds_;
        arma::Mat<double>::fixed<2, 3> redundantJointsStartToEnd_;
        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Col<double> redundantJointAnglesSine_;
        arma::Col<double> redundantJointAnglesCosine_;
    };

    //
    // Implementation
    //

    inline ParallelKinematicMachine3PRRR::ParallelKinematicMachine3PRRR() noexcept {
      setLinkLengths({
        -0.000066580445834, 0.106954081945581,
        -0.092751709777083, -0.053477040972790,
        0.092818290222917, -0.053477040972790});

      setEndEffectorJointPositions({
        0.6, 0.6,
        0.6, 0.6,
        0.6, 0.6});

      setRedundantJointPositionStarts({
        0.1, 1.0392,
        0.0, 0.8,
        1.2, 0.8
      });

      setRedundantJointPositionEnds({
        1.1, 1.0392,
        0.0, -0.2,
        1.2, -0.2});

      redundantJointPositionStartToEnds_ = redundantJointEnds_ - redundantJointStarts_;
      redundantJointIndicies_ = arma::find(arma::any(redundantJointsStartToEnd_));

      redundantJointAngleSines_.set_size(redundantJointIndicies_.n_elem);
      redundantJointAngleCosines_.set_size(redundantJointIndicies_.n_elem);

      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double redundantJointAngle = std::atan2(redundantJointPositionStartToEnds_.at(1, n), redundantJointPositionStartToEnds_.at(0, n));
        redundantJointAngleSines_.at(n) = std::sin(redundantJointAngle);
        redundantJointAngleCosines_.at(n) = std::cos(redundantJointAngle);
      }
    }

    inline std::vector<arma::Mat<double>> ParallelKinematicMachine3PRRR::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation;
    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRRR::getLinkLengths() const noexcept {
      return linkLengths_;
    }

    inline void ParallelKinematicMachine3PRRR::setLinkLengths(
        const arma::Mat<double>::fixed<2, 3>& linkLengths) noexcept {
      linkLengths_ = linkLengths;
    }

    inline arma::Col<double>::fixed<2, 3> ParallelKinematicMachine3PRRR::getEndEffectorJointPositions() const noexcept {
      return endEffectorJointPositions_;
    }

    inline void ParallelKinematicMachine3PRRR::setEndEffectorJointPositions(
        const arma::Col<double>::fixed<2, 3>& endEffectorJointPositions) noexcept {
      endEffectorJointPositions_ = endEffectorJointPositions;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRRR::getRedundantJointPositionStarts() const noexcept {
      return redundantJointPositionStarts_;
    }

    inline void ParallelKinematicMachine3PRRR::setRedundantJointPositionStarts(
        const arma::Mat<double>::fixed<2, 3>& redundantJointPositionStarts) noexcept {
      redundantJointPositionStarts_ = redundantJointPositionStarts;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRRR::getRedundantJointPositionEnds() const noexcept {
      return redundantJointPositionEnds_;
    }

    inline void ParallelKinematicMachine3PRRR::setRedundantJointPositionEnds(
        const arma::Mat<double>::fixed<2, 3>& redundantJointPositionEnds) noexcept {
      redundantJointPositionEnds_ = redundantJointPositionEnds;
    }

      if (arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::logic_error("All values for the actuation of redundantion joints must be between [0, 1].");
      }

      const arma::Col<double>::fixed<2>& endEffectorPosition = endEffectorPose.subvec(0, 1);
      const double& endEffectorAngle = endEffectorPose.at(2);

      arma::Mat<double>::fixed<2, 3> baseJoints = redundantJointStarts_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        baseJoints.col(redundantJointIndex) += redundantJointActuations.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
      }

      arma::Mat<double>::fixed<2, 3> endEffectorJoints = get2DRotationMatrix(endEffectorAngle) * endEffectorJointsRelative_;
      endEffectorJoints.each_col() += endEffectorPosition;

      arma::Mat<double>::fixed<2, 3> passiveJoints;
      for (std::size_t n = 0; n < baseJoints.n_cols; ++n) {
        passiveJoints.col(n) = getCircleCircleIntersection(baseJoints.col(n), linkLengths_.at(0, n), endEffectorJoints.col(n), linkLengths_.at(1, n));
      }

      modelCharacterisation.push_back(baseJoints);
      modelCharacterisation.push_back(endEffectorJoints);
      modelCharacterisation.push_back(passiveJoints);

      return modelCharacterisation;
    }

    inline arma::Mat<double> ParallelKinematicMachine3PRRR::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<2, 3>& baseJointPositions = modelCharacterisation.at(0);
      const arma::Mat<double>::fixed<2, 3>& passiveJointPositions = modelCharacterisation.at(2);

      const arma::Mat<double>::fixed<2, 3>& baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<3> actuation;
      for (std::size_t n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation.at(n) = std::atan2(baseToPassiveJointPositions.at(1, n), baseToPassiveJointPositions.at(0, n));
      }

      return actuation;
    }

    inline double ParallelKinematicMachine3PRRR::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<2, 3>& baseJoints = modelCharacterisation.at(0);

      const arma::Mat<double>::fixed<2, 3>& endEffectorJoints = modelCharacterisation.at(1);
      arma::Mat<double>::fixed<2, 3> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      const arma::Mat<double>::fixed<2, 3>& passiveJoints = modelCharacterisation.at(2);

      arma::Mat<double>::fixed<3, 3> forwardKinematic;
      forwardKinematic.rows(0, 1) = endEffectorJoints - passiveJoints;
      forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointsRotated.row(0);

      const arma::Mat<double>::fixed<2, 3>& baseToPassiveJoints = passiveJoints - baseJoints;
      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % baseToPassiveJoints.row(1) - forwardKinematic.row(1) % baseToPassiveJoints.row(0);
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        inverseKinematic.at(n, 3 + n) = -(forwardKinematic.at(redundantJointIndex, 0) * redundantJointAnglesCosine_.at(n) + forwardKinematic.at(redundantJointIndex, 1) * redundantJointAnglesSine_.at(n));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
