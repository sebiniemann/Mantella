namespace mant {
  namespace robotic {
    class ParallelKinematicMachine3PRPR {
      public:
        inline explicit ParallelKinematicMachine3PRPR() noexcept;

        inline arma::Row<double>::fixed<3> getMinimalActiveJointActuations() const noexcept;

        inline void setMinimalActiveJointActuations(
            const arma::Row<double>::fixed<3>& minimalActiveJointActuations) noexcept;

        inline arma::Row<double>::fixed<3> getMaximalActiveJointActuations() const noexcept;

        inline void setMaximalActiveJointActuations(
            const arma::Row<double>::fixed<3>& maximalActiveJointActuations) noexcept;

        inline std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const;
        inline arma::Mat<double>::fixed<2, 3> getEndEffectorJointPositions() const noexcept;

        inline void setEndEffectorJointPositions(
            const arma::Mat<double>::fixed<2, 3>& endEffectorJointPositions) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getRedundantJointPositionStarts() const noexcept;

        inline void setRedundantJointPositionStarts(
            const arma::Mat<double>::fixed<2, 3>& redundantJointPositionStarts) noexcept;

        inline arma::Mat<double>::fixed<2, 3> getRedundantJointPositionEnds() const noexcept;

        inline void setRedundantJointPositionEnds(
            const arma::Mat<double>::fixed<2, 3>& redundantJointPositionEnds) noexcept;

        inline arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const;

        inline double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const;

      protected:
        arma::Mat<double>::fixed<2, 3> endEffectorJointRelativePositions_;

        arma::Mat<double>::fixed<2, 3> redundantJointStartPositions_;
        arma::Mat<double>::fixed<2, 3> redundantJointEndPositions_;

        arma::Row<double>::fixed<6> minimalActiveJointActuations_;
        arma::Row<double>::fixed<6> maximalActiveJointActuations_;

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

      setRedundantJointPositionStarts({
        0.1, 1.0392,
        0.0, 0.8,
        1.2, 0.8});

      setRedundantJointPositionEnds({
        1.1, 1.0392,
        0.0, -0.2,
        1.2, -0.2});

      redundantJointStartToEndPositions_ = redundantJointEndPositions_ - redundantJointStartPositions_;
      redundantJointIndicies_ = arma::find(arma::any(redundantJointStartToEndPositions_));

      redundantJointAngleSines_.set_size(redundantJointIndicies_.n_elem);
      redundantJointAngleCosines_.set_size(redundantJointIndicies_.n_elem);

      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double redundantJointAngle = std::atan2(redundantJointStartToEndPositions_.at(1, n), redundantJointStartToEndPositions_.at(0, n));
        redundantJointAngleSines_.at(n) = std::sin(redundantJointAngle);
        redundantJointAngleCosines_.at(n) = std::cos(redundantJointAngle);
      }
    }

    inline std::vector<arma::Mat<double>> ParallelKinematicMachine3PRPR::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      std::vector<arma::Mat<double>> modelCharacterisation;
    inline arma::Row<double>::fixed<3> ParallelKinematicMachine3PRPR::getMinimalActiveJointActuations() const noexcept {
      return minimalActiveJointActuations_;
    }

    inline void ParallelKinematicMachine3PRPR::setMinimalActiveJointActuations(
        const arma::Row<double>::fixed<3>& minimalActiveJointActuations) noexcept {
      minimalActiveJointActuations_ = minimalActiveJointActuations;
    }

    inline arma::Row<double>::fixed<3> ParallelKinematicMachine3PRPR::getMaximalActiveJointActuations() const noexcept {
      return maximalActiveJointActuations_;
    }

    inline void ParallelKinematicMachine3PRPR::setMaximalActiveJointActuations(
        const arma::Row<double>::fixed<3>& maximalActiveJointActuations) noexcept {
      maximalActiveJointActuations_ = maximalActiveJointActuations;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRPR::getEndEffectorJointPositions() const noexcept {
      return endEffectorJointPositions_;
    }

    inline void ParallelKinematicMachine3PRPR::setEndEffectorJointPositions(
        const arma::Mat<double>::fixed<2, 3>& endEffectorJointPositions) noexcept {
      endEffectorJointPositions_ = endEffectorJointPositions;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRPR::getRedundantJointPositionStarts() const noexcept {
      return redundantJointStartPositions_;
    }

    inline void ParallelKinematicMachine3PRPR::setRedundantJointPositionStarts(
        const arma::Mat<double>::fixed<2, 3>& redundantJointStartPositions) noexcept {
      redundantJointStartPositions_ = redundantJointStartPositions;
    }

    inline arma::Mat<double>::fixed<2, 3> ParallelKinematicMachine3PRPR::getRedundantJointPositionEnds() const noexcept {
      return redundantJointEndPositions_;
    }

    inline void ParallelKinematicMachine3PRPR::setRedundantJointPositionEnds(
        const arma::Mat<double>::fixed<2, 3>& redundantJointEndPositions) noexcept {
      redundantJointEndPositions_ = redundantJointEndPositions;
    }

      if (arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::logic_error("All values for the actuation of redundantion joints must be between [0, 1].");
      }

      const arma::Col<double>::fixed<2>& endEffectorPosition = endEffectorPose.subvec(0, 1);
      const double& endEffectorAngle = endEffectorPose.at(2);

      arma::Mat<double>::fixed<2, 3> baseJointPositions = redundantJointStartPositions_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        baseJointPositions.col(redundantJointIndex) += redundantJointActuations.at(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      arma::Mat<double>::fixed<2, 3> endEffectorJointPositions = get2DRotationMatrix(endEffectorAngle) * endEffectorJointRelativePositions_;
      endEffectorJointPositions.each_col() += endEffectorPosition;

      modelCharacterisation.push_back(baseJointPositions);
      modelCharacterisation.push_back(endEffectorJointPositions);

      return modelCharacterisation;
    }

    inline arma::Mat<double> ParallelKinematicMachine3PRPR::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<2, 3>& baseJoints = modelCharacterisation.at(0);
      const arma::Mat<double>::fixed<2, 3>& endEffectorJoints = modelCharacterisation.at(1);

      return arma::sqrt(arma::sum(arma::square(endEffectorJoints - baseJoints)));
    }

    inline double ParallelKinematicMachine3PRPR::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      const std::vector<arma::Mat<double>>& modelCharacterisation = getModelCharacterisation(endEffectorPose, redundantJointActuations);

      const arma::Mat<double>::fixed<2, 3>& baseJointPositions = modelCharacterisation.at(1);

      const arma::Mat<double>::fixed<2, 3>& endEffectorJointPositions = modelCharacterisation.at(1);
      arma::Mat<double>::fixed<2, 3> endEffectorJointRelativePositionsRotated = endEffectorJointPositions;
      endEffectorJointRelativePositionsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      const arma::Mat<double>::fixed<2, 3>& baseToEndEffectorJointPositions = endEffectorJointPositions - baseJointPositions;
      const arma::Row<double>::fixed<3>& baseToEndEffectorJointActuations = arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));

      if (arma::any(baseToEndEffectorJointActuations < minimalActiveJointActuations_) || arma::any(baseToEndEffectorJointActuations > maximalActiveJointActuations_)) {
        return 0.0;
      }

      arma::Mat<double>::fixed<3, 3> forwardKinematic;
      forwardKinematic.rows(0, 1) = baseToEndEffectorJointPositions;
      forwardKinematic.row(2) = -forwardKinematic.row(0) % endEffectorJointRelativePositionsRotated.row(1) + forwardKinematic.row(1) % endEffectorJointRelativePositionsRotated.row(0);

      arma::Mat<double> inverseKinematic(3, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = -baseToEndEffectorJointActuations;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        inverseKinematic.at(n, 3 + n) = forwardKinematic.at(redundantJointIndex, 0) * redundantJointAngleCosines_.at(n) + forwardKinematic.at(redundantJointIndex, 1) * redundantJointAngleSines_.at(n);
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
  }
}
