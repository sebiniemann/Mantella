namespace mant {
  namespace robotic {
    class ParallelKinematicMachine3PRPR {
      public:
        // TODO Add end effector rotation
        inline explicit ParallelKinematicMachine3PRPR() noexcept;

        inline arma::Row<double>::fixed<3> getMinimalActiveJointActuations() const noexcept;

        inline void setMinimalActiveJointActuations(
            const arma::Row<double>::fixed<3> minimalActiveJointActuations) noexcept;

        inline arma::Row<double>::fixed<3> getMaximalActiveJointActuations() const noexcept;

        inline void setMaximalActiveJointActuations(
            const arma::Row<double>::fixed<3> maximalActiveJointActuations) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getEndEffectorJointPositions() const noexcept;

        inline void setEndEffectorJointPositions(
            const arma::Mat<double>::fixed<2, 3> endEffectorJointPositions) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getRedundantJointStartPositions() const noexcept;

        inline void setRedundantJointStartPositions(
            const arma::Mat<double>::fixed<2, 3> redundantJointStartPositions) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getRedundantJointEndPositions() const noexcept;

        inline void setRedundantJointEndPositions(
            const arma::Mat<double>::fixed<2, 3> redundantJointEndPositions) noexcept;

        inline arma::Cube<double>::fixed<2, 3, 2> getModel(
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

      protected:
        arma::Row<double>::fixed<3> minimalActiveJointActuations_;
        arma::Row<double>::fixed<3> maximalActiveJointActuations_;

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

    inline ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR() noexcept {
      setMinimalActiveJointActuations({0.1, 0.1, 0.1});
      setMinimalActiveJointActuations({1.2, 1.2, 1.2});

      setEndEffectorJointPositions({
        -0.000066580445834, 0.106954081945581,
        -0.092751709777083, -0.053477040972790,
        0.092818290222917, -0.053477040972790});

      setRedundantJointStartPositions({
        0.1, 1.0392,
        0.0, 0.8,
        1.2, 0.8});

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

    inline arma::Row<double>::fixed<3> ParallelKinematicMachine3PRPR::getMinimalActiveJointActuations() const noexcept {
      return minimalActiveJointActuations_;
    }

    inline void ParallelKinematicMachine3PRPR::setMinimalActiveJointActuations(
        const arma::Row<double>::fixed<3> minimalActiveJointActuations) noexcept {
      minimalActiveJointActuations_ = minimalActiveJointActuations;
    }

    inline arma::Row<double>::fixed<3> ParallelKinematicMachine3PRPR::getMaximalActiveJointActuations() const noexcept {
      return maximalActiveJointActuations_;
    }

    inline void ParallelKinematicMachine3PRPR::setMaximalActiveJointActuations(
        const arma::Row<double>::fixed<3> maximalActiveJointActuations) noexcept {
      maximalActiveJointActuations_ = maximalActiveJointActuations;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRPR::getEndEffectorJointPositions() const noexcept {
      return endEffectorJointPositions_;
    }

    inline void ParallelKinematicMachine3PRPR::setEndEffectorJointPositions(
        const arma::Mat<double>::fixed<2, 3> endEffectorJointPositions) noexcept {
      endEffectorJointPositions_ = endEffectorJointPositions;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRPR::getRedundantJointStartPositions() const noexcept {
      return redundantJointStartPositions_;
    }

    inline void ParallelKinematicMachine3PRPR::setRedundantJointStartPositions(
        const arma::Mat<double>::fixed<2, 3> redundantJointStartPositions) noexcept {
      redundantJointStartPositions_ = redundantJointStartPositions;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRPR::getRedundantJointEndPositions() const noexcept {
      return redundantJointEndPositions_;
    }

    inline void ParallelKinematicMachine3PRPR::setRedundantJointEndPositions(
        const arma::Mat<double>::fixed<2, 3> redundantJointEndPositions) noexcept {
      redundantJointEndPositions_ = redundantJointEndPositions;
    }

    inline arma::Cube<double>::fixed<2, 3, 2> ParallelKinematicMachine3PRPR::getModel(
        const arma::Col<double>::fixed<3>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the actuation of redundantion joints must be between [0, 1].");
      verify(redundantJointActuations.n_elem == redundantJointIndicies_.n_elem, "The number of redundant actuations must be equal to the number of redundant joints.");

      arma::Cube<double>::fixed<2, 3, 2> model;

      const arma::Col<double>::fixed<2>& endEffectorPosition = endEffectorPose.subvec(0, 1);
      const double& endEffectorAngle = endEffectorPose(2);

      model.slice(0) = redundantJointStartPositions_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        model.slice(0).col(redundantJointIndex) += redundantJointActuations(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      model.slice(1) = get2DRotation(endEffectorAngle) * endEffectorJointPositions_;
      model.slice(1).each_col() += endEffectorPosition;

      return model;
    }

    inline arma::Row<double>::fixed<3> ParallelKinematicMachine3PRPR::getActuation(
        const arma::Col<double>::fixed<3>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<2, 3, 2>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<2, 3>& baseJoints = model.slice(0);
      const arma::Mat<double>::fixed<2, 3>& endEffectorJoints = model.slice(1);

      return arma::sqrt(arma::sum(arma::square(endEffectorJoints - baseJoints)));
    }

    inline arma::Col<double>::fixed<3> ParallelKinematicMachine3PRPR::getEndEffectorPose(
        const arma::Row<double>::fixed<3>& actuations,
        const arma::Row<double>& redundantJointActuations) const {
      // TODO Direct kinematic (estimate position, using a simple HillCLimber algorithm)
      return {0, 0, 0};
    }

    inline double ParallelKinematicMachine3PRPR::getEndEffectorPoseError(
        const arma::Col<double>::fixed<3>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<2, 3, 2>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<2, 3>& baseJointPositions = model.slice(1);

      const arma::Mat<double>::fixed<2, 3>& endEffectorJointPositions = model.slice(1);
      arma::Mat<double>::fixed<2, 3> endEffectorJointPositionsRotated = endEffectorJointPositions;
      endEffectorJointPositionsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      const arma::Mat<double>::fixed<2, 3>& baseToEndEffectorJointPositions = endEffectorJointPositions - baseJointPositions;
      const arma::Row<double>::fixed<3>& baseToEndEffectorJointActuations = arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));

      if (arma::any(baseToEndEffectorJointActuations < minimalActiveJointActuations_) || arma::any(baseToEndEffectorJointActuations > maximalActiveJointActuations_)) {
        return 0.0;
      }

      arma::Mat<double>::fixed<3, 3> forwardKinematic;
      forwardKinematic.head_rows(2) = baseToEndEffectorJointPositions;
      forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointPositionsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointPositionsRotated.row(0);

      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = -baseToEndEffectorJointActuations;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        inverseKinematic(n, 3 + n) = forwardKinematic(redundantJointIndex, 0) * redundantJointAngleCosines_(n) + forwardKinematic(redundantJointIndex, 1) * redundantJointAngleSines_(n);
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
