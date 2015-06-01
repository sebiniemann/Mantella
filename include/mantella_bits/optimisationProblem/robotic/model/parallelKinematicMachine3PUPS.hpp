namespace mant {
  namespace robotic {
    class ParallelKinematicMachine3PUPS {
      public:
        inline explicit ParallelKinematicMachine3PUPS() noexcept;

        inline arma::Row<double>::fixed<3> getMinimalActiveJointActuations() const noexcept;

        inline void setMinimalActiveJointActuations(
            const arma::Row<double>::fixed<3> minimalActiveJointActuations) noexcept;

        inline arma::Row<double>::fixed<3> getMaximalActiveJointActuations() const noexcept;

        inline void setMaximalActiveJointActuations(
            const arma::Row<double>::fixed<3> maximalActiveJointActuations) noexcept;

        inline arma::Mat<double>::fixed<3, 3> getEndEffectorJointPositions() const noexcept;

        inline void setEndEffectorJointPositions(
            const arma::Mat<double>::fixed<3, 3> endEffectorJointPositions) noexcept;

        inline arma::Mat<double>::fixed<3, 3> getRedundantJointStartPositions() const noexcept;

        inline void setRedundantJointStartPositions(
            const arma::Mat<double>::fixed<3, 3> redundantJointStartPositions) noexcept;

        inline arma::Mat<double>::fixed<3, 3> getRedundantJointEndPositions() const noexcept;

        inline void setRedundantJointEndPositions(
            const arma::Mat<double>::fixed<3, 3> redundantJointEndPositions) noexcept;

        inline arma::Cube<double>::fixed<3, 3, 2> getModel(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

        inline arma::Row<double>::fixed<3> getActuation(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

        inline arma::Col<double>::fixed<6> getEndEffectorPose(
            const arma::Row<double>::fixed<3>& actuations,
            const arma::Row<double>::fixed<3>& endEffectorRotation,
            const arma::Row<double>& redundantJointActuations) const;

        inline double getEndEffectorPoseError(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Row<double>& redundantJointActuations) const;

      protected:
        arma::Row<double>::fixed<3> minimalActiveJointActuations_;
        arma::Row<double>::fixed<3> maximalActiveJointActuations_;

        arma::Mat<double>::fixed<3, 3> endEffectorJointPositions_;

        arma::Mat<double>::fixed<3, 3> redundantJointStartPositions_;
        arma::Mat<double>::fixed<3, 3> redundantJointEndPositions_;

        arma::Mat<double>::fixed<3, 3> redundantJointStartToEndPositions_;

        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };

    //
    // Implementation
    //

    inline ParallelKinematicMachine3PUPS::ParallelKinematicMachine3PUPS() noexcept {
      setMinimalActiveJointActuations({0.39, 0.39, 0.39});
      setMaximalActiveJointActuations({0.95, 0.95, 0.95});

      setEndEffectorJointPositions({
        -0.025561381023353, 0.086293776138137, 0.12,
        0.087513292835791, -0.021010082747031, 0.12,
        -0.061951911812438, -0.065283693391106, 0.12});

      setRedundantJointStartPositions({
        -0.463708870031622, 0.417029254828353, -0.346410161513775,
        0.593012363818459, 0.193069033993384, -0.346410161513775,
        -0.129303493786837, -0.610098288821738, -0.346410161513775});

      setRedundantJointEndPositions({
        -0.247202519085512, 0.292029254828353, 0.086602540378444,
        0.376506012872349, 0.068069033993384, 0.086602540378444,
        -0.129303493786837, -0.360098288821738, 0.086602540378444});

      redundantJointStartToEndPositions_ = redundantJointEndPositions_ - redundantJointStartPositions_;
      redundantJointIndicies_ = arma::find(arma::any(redundantJointStartToEndPositions_));

      redundantJointAngles_.set_size(3, redundantJointIndicies_.n_elem);

      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double& redundantJointXAngle = std::atan2(redundantJointStartToEndPositions_(1, n), redundantJointStartToEndPositions_(0, n));
        const double& redundantJointYAngle = std::atan2(redundantJointStartToEndPositions_(2, n), redundantJointStartToEndPositions_(1, n));
        redundantJointAngles_.col(n) = arma::Col<double>::fixed<3>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }
    }

    inline arma::Row<double>::fixed<3> ParallelKinematicMachine3PUPS::getMinimalActiveJointActuations() const noexcept {
      return minimalActiveJointActuations_;
    }

    inline void ParallelKinematicMachine3PUPS::setMinimalActiveJointActuations(
        const arma::Row<double>::fixed<3> minimalActiveJointActuations) noexcept {
      minimalActiveJointActuations_ = minimalActiveJointActuations;
    }

    inline arma::Row<double>::fixed<3> ParallelKinematicMachine3PUPS::getMaximalActiveJointActuations() const noexcept {
      return maximalActiveJointActuations_;
    }

    inline void ParallelKinematicMachine3PUPS::setMaximalActiveJointActuations(
        const arma::Row<double>::fixed<3> maximalActiveJointActuations) noexcept {
      maximalActiveJointActuations_ = maximalActiveJointActuations;
    }

    inline arma::Mat<double>::fixed<3, 3> ParallelKinematicMachine3PUPS::getEndEffectorJointPositions() const noexcept {
      return endEffectorJointPositions_;
    }

    inline void ParallelKinematicMachine3PUPS::setEndEffectorJointPositions(
        const arma::Mat<double>::fixed<3, 3> endEffectorJointPositions) noexcept {
      endEffectorJointPositions_ = endEffectorJointPositions;
    }

    inline arma::Mat<double>::fixed<3, 3> ParallelKinematicMachine3PUPS::getRedundantJointStartPositions() const noexcept {
      return redundantJointStartPositions_;
    }

    inline void ParallelKinematicMachine3PUPS::setRedundantJointStartPositions(
        const arma::Mat<double>::fixed<3, 3> redundantJointStartPositions) noexcept {
      redundantJointStartPositions_ = redundantJointStartPositions;
    }

    inline arma::Mat<double>::fixed<3, 3> ParallelKinematicMachine3PUPS::getRedundantJointEndPositions() const noexcept {
      return redundantJointEndPositions_;
    }

    inline void ParallelKinematicMachine3PUPS::setRedundantJointEndPositions(
        const arma::Mat<double>::fixed<3, 3> redundantJointEndPositions) noexcept {
      redundantJointEndPositions_ = redundantJointEndPositions;
    }

    inline arma::Cube<double>::fixed<3, 3, 2> ParallelKinematicMachine3PUPS::getModel(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the actuation of redundantion joints must be between [0, 1].");
      verify(redundantJointActuations.n_elem == redundantJointIndicies_.n_elem, "The number of redundant actuations must be equal to the number of redundant joints.");

      arma::Cube<double>::fixed<3, 3, 2> model;

      const arma::Col<double>::fixed<3>& endEffectorPosition = endEffectorPose.subvec(0, 2);
      const double& endEffectorRollAngle = endEffectorPose(3);
      const double& endEffectorPitchAngle = endEffectorPose(4);
      const double& endEffectorYawAngle = endEffectorPose(5);

      model.slice(0) = redundantJointStartPositions_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        model.slice(0).col(redundantJointIndex) += redundantJointActuations(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      model.slice(1) = get3DRotation(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointPositions_;
      model.slice(1).each_col() += endEffectorPosition;

      return model;
    }

    inline arma::Col<double>::fixed<6> ParallelKinematicMachine3PUPS::getEndEffectorPose(
        const arma::Row<double>::fixed<3>& actuations,
        const arma::Row<double>::fixed<3>& endEffectorRotation,
        const arma::Row<double>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the actuation of redundantion joints must be between [0, 1].");
      verify(redundantJointActuations.n_elem == redundantJointIndicies_.n_elem, "The number of redundant actuations must be equal to the number of redundant joints.");

      const double& endEffectorRollAngle = endEffectorRotation(0);
      const double& endEffectorPitchAngle = endEffectorRotation(1);
      const double& endEffectorYawAngle = endEffectorRotation(2);

      arma::Mat<double> baseJointPositions = redundantJointStartPositions_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        baseJointPositions.col(redundantJointIndex) += redundantJointActuations(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      baseJointPositions -= get3DRotation(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointPositions_;

      return arma::join_cols(getTriangulation(baseJointPositions.col(0), actuations(0), baseJointPositions.col(1), actuations(1), baseJointPositions.col(2), actuations(2)), endEffectorRotation);
    }

    inline arma::Row<double>::fixed<3> ParallelKinematicMachine3PUPS::getActuation(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<3, 3, 2>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 3>& baseJoints = model.slice(0);
      const arma::Mat<double>::fixed<3, 3>& endEffectorJoints = model.slice(1);

      return arma::sqrt(arma::sum(arma::square(endEffectorJoints - baseJoints)));
    }

    inline double ParallelKinematicMachine3PUPS::getEndEffectorPoseError(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Row<double>& redundantJointActuations) const {
      const arma::Cube<double>::fixed<3, 3, 2>& model = getModel(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<3, 3>& baseJoints = model.slice(0);

      const arma::Mat<double>::fixed<3, 3>& endEffectorJoints = model.slice(1);
      arma::Mat<double>::fixed<3, 3> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      const arma::Mat<double>::fixed<3, 3>& baseToEndEffectorJointPositions = endEffectorJoints - baseJoints;
      const arma::Row<double>::fixed<3>& baseToEndEffectorJointActuations = arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));

      if (arma::any(baseToEndEffectorJointActuations < minimalActiveJointActuations_) || arma::any(baseToEndEffectorJointActuations > maximalActiveJointActuations_)) {
        return 0.0;
      }

      arma::Mat<double>::fixed<6, 3> forwardKinematic;
      forwardKinematic.head_rows(3) = baseToEndEffectorJointPositions;
      for (std::size_t n = 0; n < baseToEndEffectorJointPositions.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJointPositions.col(n));
      }

      arma::Mat<double> inverseKinematic(6, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_(n);
        inverseKinematic(n, 3 + n) = arma::dot(baseToEndEffectorJointPositions.col(redundantJointIndex), redundantJointAngles_.col(redundantJointIndex));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
