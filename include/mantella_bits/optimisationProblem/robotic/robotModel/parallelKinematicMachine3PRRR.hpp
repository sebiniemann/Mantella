namespace mant {
  namespace robotic {
    class ParallelKinematicMachine3PRRR {
      public:
        // TODO Add end effector rotation
        inline explicit ParallelKinematicMachine3PRRR() noexcept;

        inline arma::Mat<double>::fixed<2, 3> getLinkLengths() const noexcept;

        inline void setLinkLengths(
            const arma::Mat<double>::fixed<2, 3> linkLengths) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getEndEffectorJointPositions() const noexcept;

        inline void setEndEffectorJointPositions(
            const arma::Mat<double>::fixed<2, 3> endEffectorJointPositions) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getRedundantJointStartPositions() const noexcept;

        inline void setRedundantJointStartPositions(
            const arma::Mat<double>::fixed<2, 3> redundantJointStartPositions) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getRedundantJointEndPositions() const noexcept;

        inline void setRedundantJointEndPositions(
            const arma::Mat<double>::fixed<2, 3> redundantJointEndPositions) noexcept;

        inline arma::Cube<double>::fixed<2, 3, 3> getModel(
            const arma::Col<double>::fixed<3>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

        inline arma::Row<double>::fixed<3> getActuation(
            const arma::Col<double>::fixed<3>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

        inline arma::Col<double>::fixed<3> getEndEffectorPose(
            const arma::Row<double>::fixed<3>& actuations,
            const arma::Row<double>& redundantJointActuations) const;

        inline double getEndEffectorPoseError(
            const arma::Col<double>::fixed<3>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

        arma::Mat<double>::fixed<2, 3> linkLengths_;

        arma::Mat<double>::fixed<2, 3> endEffectorJointPositions_;

        arma::Mat<double>::fixed<2, 3> redundantJointStartPositions_;
        arma::Mat<double>::fixed<2, 3> redundantJointEndPositions_;

        arma::Mat<double>::fixed<2, 3> redundantJointStartToEndPositions_;

        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Row<double> redundantJointAngleSines_;
        arma::Row<double> redundantJointAngleCosines_;
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

      setRedundantJointStartPositions({
        0.1, 1.0392,
        0.0, 0.8,
        1.2, 0.8
      });

      setRedundantJointEndPositions({
        1.1, 1.0392,
        0.0, -0.2,
        1.2, -0.2});

      redundantJointStartToEndPositions_ = redundantJointEndPositions_ - redundantJointStartPositions_;
      redundantJointIndicies_ = arma::find(arma::any(redundantJointStartToEndPositions_));

      redundantJointAngleSines_.set_size(redundantJointIndicies_.n_elem);
      redundantJointAngleCosines_.set_size(redundantJointIndicies_.n_elem);

      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double redundantJointAngle = std::atan2(redundantJointStartToEndPositions_(1, n), redundantJointStartToEndPositions_(0, n));
        redundantJointAngleSines_(n) = std::sin(redundantJointAngle);
        redundantJointAngleCosines_(n) = std::cos(redundantJointAngle);
      }
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRRR::getLinkLengths() const noexcept {
      return linkLengths_;
    }

    inline void ParallelKinematicMachine3PRRR::setLinkLengths(
        const arma::Mat<double>::fixed<2, 3> linkLengths) noexcept {
      linkLengths_ = linkLengths;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRRR::getEndEffectorJointPositions() const noexcept {
      return endEffectorJointPositions_;
    }

    inline void ParallelKinematicMachine3PRRR::setEndEffectorJointPositions(
        const arma::Mat<double>::fixed<2, 3> endEffectorJointPositions) noexcept {
      endEffectorJointPositions_ = endEffectorJointPositions;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRRR::getRedundantJointStartPositions() const noexcept {
      return redundantJointStartPositions_;
    }

    inline void ParallelKinematicMachine3PRRR::setRedundantJointStartPositions(
        const arma::Mat<double>::fixed<2, 3> redundantJointStartPositions) noexcept {
      redundantJointStartPositions_ = redundantJointStartPositions;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRRR::getRedundantJointEndPositions() const noexcept {
      return redundantJointEndPositions_;
    }

    inline void ParallelKinematicMachine3PRRR::setRedundantJointEndPositions(
        const arma::Mat<double>::fixed<2, 3> redundantJointEndPositions) noexcept {
      redundantJointEndPositions_ = redundantJointEndPositions;
    }

    inline arma::Cube<double>::fixed<2, 3, 3> ParallelKinematicMachine3PRRR::getModel(
        const arma::Col<double>::fixed<3>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the actuation of redundantion joints must be between [0, 1].");
      verify(redundantJointActuations.n_elem == redundantJointIndicies_.n_elem, "The number of redundant actuations must be equal to the number of redundant joints.");

      arma::Cube<double>::fixed<2, 3, 3> model;

      const arma::Col<double>::fixed<2>& endEffectorPosition = endEffectorPose.subvec(0, 1);
      const double& endEffectorAngle = endEffectorPose(2);

      model.slice(0) = redundantJointStartPositions_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        model.slice(0).col(redundantJointIndex) += redundantJointActuations(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      model.slice(2) = get2DRotation(endEffectorAngle) * endEffectorJointPositions_;
      model.slice(2).each_col() += endEffectorPosition;

      for (std::size_t n = 0; n < model.slice(0).n_cols; ++n) {
        model.slice(1).col(n) = getCircleCircleIntersection(model.slice(0).col(n), linkLengths_(0, n), model.slice(2).col(n), linkLengths_(1, n));
      }

      return model;
    }

    inline arma::Row<double>::fixed<3> ParallelKinematicMachine3PRRR::getActuation(
        const arma::Col<double>::fixed<3>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<2, 3, 3>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<2, 3>& baseJointPositions = model.slice(0);
      const arma::Mat<double>::fixed<2, 3>& passiveJointPositions = model.slice(1);

      const arma::Mat<double>::fixed<2, 3>& baseToPassiveJointPositions = passiveJointPositions - baseJointPositions;

      arma::Row<double>::fixed<3> actuation;
      for (std::size_t n = 0; n < baseToPassiveJointPositions.n_elem; ++n) {
        actuation(n) = std::atan2(baseToPassiveJointPositions(1, n), baseToPassiveJointPositions(0, n));
      }

      return actuation;
    }

    inline arma::Col<double>::fixed<3> ParallelKinematicMachine3PRRR::getEndEffectorPose(
        const arma::Row<double>::fixed<3>& actuations,
        const arma::Row<double>& redundantJointActuations) const {
      // TODO Direct kinematic (estimate position, using a simple HillCLimber algorithm)
      return {0, 0, 0};
    }

    inline double ParallelKinematicMachine3PRRR::getEndEffectorPoseError(
        const arma::Col<double>::fixed<3>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<2, 3, 3>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<2, 3>& baseJoints = model.slice(0);

      const arma::Mat<double>::fixed<2, 3>& endEffectorJoints = model.slice(2);
      arma::Mat<double>::fixed<2, 3> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      const arma::Mat<double>::fixed<2, 3>& passiveJoints = model.slice(1);

      arma::Mat<double>::fixed<3, 3> forwardKinematic;
      forwardKinematic.head_rows(2) = endEffectorJoints - passiveJoints;
      forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointsRotated.row(0);

      const arma::Mat<double>::fixed<2, 3>& baseToPassiveJoints = passiveJoints - baseJoints;
      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = forwardKinematic.row(0) % baseToPassiveJoints.row(1) - forwardKinematic.row(1) % baseToPassiveJoints.row(0);
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        inverseKinematic(n, 3 + n) = -(forwardKinematic(redundantJointIndex, 0) * redundantJointAngleCosines_(n) + forwardKinematic(redundantJointIndex, 1) * redundantJointAngleSines_(n));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
