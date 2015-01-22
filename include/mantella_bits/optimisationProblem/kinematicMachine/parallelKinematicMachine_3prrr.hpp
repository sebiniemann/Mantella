namespace mant {
  namespace robotic {
    class ParallelKinematicMachine_3PRRR : public KinematicMachine {
      public:
        inline explicit ParallelKinematicMachine_3PRRR() noexcept;
        inline explicit ParallelKinematicMachine_3PRRR(
            const arma::Mat<double>::fixed<2, 3>& relativeEndEffectorJoints,
            const arma::Mat<double>::fixed<2, 3>& linkLengths,
            const arma::Mat<double>::fixed<2, 3>& redundantJointStarts,
            const arma::Mat<double>::fixed<2, 3>& redundantJointEnds) noexcept;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        ParallelKinematicMachine_3PRRR(const ParallelKinematicMachine_3PRRR&) = delete;
        ParallelKinematicMachine_3PRRR& operator=(const ParallelKinematicMachine_3PRRR&) = delete;

        inline std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        inline arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        inline double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        arma::Mat<double>::fixed<2, 3> endEffectorJointsRelative_;
        arma::Mat<double>::fixed<2, 3> linkLengths_;

        arma::Mat<double>::fixed<2, 3> redundantJointStarts_;
        arma::Mat<double>::fixed<2, 3> redundantJointEnds_;
        arma::Mat<double>::fixed<2, 3> redundantJointsStartToEnd_;
        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Col<double> redundantJointAnglesSine_;
        arma::Col<double> redundantJointAnglesCosine_;
    };

    inline ParallelKinematicMachine_3PRRR::ParallelKinematicMachine_3PRRR() noexcept
      : ParallelKinematicMachine_3PRRR(
          arma::Mat<double>::fixed<2, 3>({
            -0.000066580445834, 0.106954081945581,
            -0.092751709777083, -0.053477040972790,
            0.092818290222917, -0.053477040972790}),
          arma::Mat<double>::fixed<2, 3>({
            0.6, 0.6,
            0.6, 0.6,
            0.6, 0.6}),
          arma::Mat<double>::fixed<2, 3>({
            0.1, 1.0392,
            0.0, 0.8,
            1.2, 0.8}),
          arma::Mat<double>::fixed<2, 3>({
            1.1, 1.0392,
            0.0, -0.2,
            1.2, -0.2})) {

    }

    inline ParallelKinematicMachine_3PRRR::ParallelKinematicMachine_3PRRR(
        const arma::Mat<double>::fixed<2, 3>& relativeEndEffectorJoints,
        const arma::Mat<double>::fixed<2, 3>& linkLengths,
        const arma::Mat<double>::fixed<2, 3>& redundantJointStarts,
        const arma::Mat<double>::fixed<2, 3>& redundantJointEnds) noexcept
      : endEffectorJointsRelative_(relativeEndEffectorJoints),
        linkLengths_(linkLengths),
        redundantJointStarts_(redundantJointStarts),
        redundantJointEnds_(redundantJointEnds),
        redundantJointsStartToEnd_(redundantJointEnds_ - redundantJointStarts_),
        redundantJointIndicies_(arma::find(arma::any(redundantJointsStartToEnd_))),
        redundantJointAnglesSine_(redundantJointIndicies_.n_elem),
        redundantJointAnglesCosine_(redundantJointIndicies_.n_elem) {
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double redundantJointAngle = std::atan2(redundantJointsStartToEnd_.at(1, n), redundantJointsStartToEnd_.at(0, n));
        redundantJointAnglesSine_.at(n) = std::sin(redundantJointAngle);
        redundantJointAnglesCosine_.at(n) = std::cos(redundantJointAngle);
      }
    }

    inline std::vector<arma::Mat<double>> ParallelKinematicMachine_3PRRR::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisation;

      if (arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1))) {
        throw std::runtime_error("All values for the actuation of redundantion joints must be between [0, 1].");
      }

      const arma::Col<double>::fixed<2>& endEffector = endEffectorPose.subvec(0, 1);
      const double& endEffectorAngle = endEffectorPose.at(2);

      arma::Mat<double>::fixed<2, 3> baseJoints = redundantJointStarts_;
      for (std::size_t n = 0; n < redundantJointIndicies_.n_elem; n++) {
        const unsigned int& redundantJointIndex = redundantJointIndicies_.at(n);
        baseJoints.col(redundantJointIndex) += redundantJointActuations.at(redundantJointIndex) * redundantJointsStartToEnd_.col(redundantJointIndex);
      }

      arma::Mat<double>::fixed<2, 3> endEffectorJoints = get2DRotationMatrix(endEffectorAngle) * endEffectorJointsRelative_;
      endEffectorJoints.each_col() += endEffector;

      arma::Mat<double>::fixed<2, 3> passiveJoints;
      for (std::size_t n = 0; n < baseJoints.n_cols; ++n) {
        passiveJoints.col(n) = getCircleCircleIntersection(baseJoints.col(n), linkLengths_.at(0, n), endEffectorJoints.col(n), linkLengths_.at(1, n));
      }

      modelCharacterisation.push_back(baseJoints);
      modelCharacterisation.push_back(endEffectorJoints);
      modelCharacterisation.push_back(passiveJoints);

      return modelCharacterisation;
    }

    inline arma::Mat<double> ParallelKinematicMachine_3PRRR::getActuation(
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

    inline double ParallelKinematicMachine_3PRRR::getPositionError(
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
