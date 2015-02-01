namespace mant {
  namespace robotic {
    class ParallelKinematicMachine3PRPR : public KinematicMachine {
      public:
        inline explicit ParallelKinematicMachine3PRPR() noexcept;
        inline explicit ParallelKinematicMachine3PRPR(
            const arma::Mat<double>::fixed<2, 3>& endEffectorJointRelativePositions,
            const arma::Mat<double>::fixed<2, 3>& redundantJointStartPositions,
            const arma::Mat<double>::fixed<2, 3>& redundantJointEndPositions,
            const arma::Row<double>::fixed<3>& minimalActiveJointActuations,
            const arma::Row<double>::fixed<3>& maximalActiveJointActuations) noexcept;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        ParallelKinematicMachine3PRPR(const ParallelKinematicMachine3PRPR&) = delete;
        ParallelKinematicMachine3PRPR& operator=(const ParallelKinematicMachine3PRPR&) = delete;

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

    inline ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR() noexcept
      : ParallelKinematicMachine3PRPR(
          arma::Mat<double>::fixed<2, 3>({
            -0.000066580445834, 0.106954081945581,
            -0.092751709777083, -0.053477040972790,
            0.092818290222917, -0.053477040972790}),
          arma::Mat<double>::fixed<2, 3>({
            0.1, 1.0392,
            0.0, 0.8,
            1.2, 0.8}),
          arma::Mat<double>::fixed<2, 3>({
            1.1, 1.0392,
            0.0, -0.2,
            1.2, -0.2}),
          arma::Row<double>::fixed<3>({
            0.1, 0.1, 0.1}),
          arma::Row<double>::fixed<3>({
            1.2, 1.2, 1.2})) {

    }

    inline ParallelKinematicMachine3PRPR::ParallelKinematicMachine3PRPR(
        const arma::Mat<double>::fixed<2, 3>& endEffectorJointRelativePositions,
        const arma::Mat<double>::fixed<2, 3>& redundantJointStarts,
        const arma::Mat<double>::fixed<2, 3>& redundantJointEnds,
        const arma::Row<double>::fixed<3>& minimalActiveJointActuations,
        const arma::Row<double>::fixed<3>& maximalActiveJointActuations) noexcept
      : endEffectorJointRelativePositions_(endEffectorJointRelativePositions),
        redundantJointStartPositions_(redundantJointStarts),
        redundantJointEndPositions_(redundantJointEnds),
        minimalActiveJointActuations_(minimalActiveJointActuations),
        maximalActiveJointActuations_(maximalActiveJointActuations),
        redundantJointStartToEndPositions_(redundantJointEndPositions_ - redundantJointStartPositions_),
        redundantJointIndicies_(arma::find(arma::any(redundantJointStartToEndPositions_))),
        redundantJointAngleSines_(redundantJointIndicies_.n_elem),
        redundantJointAngleCosines_(redundantJointIndicies_.n_elem) {
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