namespace mant {
  namespace robotic {
    class MultiLevelStewartPlatform {
      public:
        inline explicit MultiLevelStewartPlatform() noexcept;

        inline void setPlatformLevels(
            const std::vector<ParallelKinematicMachine6PUPS> platformLevels) noexcept;

        inline std::vector<arma::Cube<double>::fixed<3, 6, 2>> getModel(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const;

        inline arma::Mat<double> getActuation(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const;

        inline double getEndEffectorPoseError(
            const arma::Col<double>::fixed<6>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const;

      protected:
        std::vector<ParallelKinematicMachine6PUPS> platformLevels_;
    };

    //
    // Implementation
    //

    inline MultiLevelStewartPlatform::MultiLevelStewartPlatform() noexcept {
      ParallelKinematicMachine6PUPS firstPlatformLevel;
      firstPlatformLevel.setMinimalActiveJointActuations({0.2148, 0.2148, 0.2148, 0.2148, 0.2148, 0.2148});
      firstPlatformLevel.setMaximalActiveJointActuations({0.3148, 0.3148, 0.3148, 0.3148, 0.3148, 0.3148});
      firstPlatformLevel.setEndEffectorJointPositions({
        0.0302769856567722, -0.0664251770004387, -0.009,
        0.0423873979048716, -0.0594332272290012, -0.009,
        0.0423873979048716, 0.0594332272290012, -0.009,
        0.0302769856567722, 0.0664251770004387, -0.009,
        -0.0726643835616438, 0.0069919497714374, -0.009,
        -0.0726643835616438, -0.0069919497714374, -0.009});
      firstPlatformLevel.setRedundantJointStartPositions({
        -0.0302769856567722, -0.0664251770004387, 0.009,
        0.0726643835616438, -0.0069919497714374, 0.009,
        0.0726643835616438, 0.0069919497714374, 0.009,
        -0.0302769856567722, 0.0664251770004387, 0.009,
        -0.0423873979048716, 0.0594332272290012, 0.009,
        -0.0423873979048716,-0.0594332272290012, 0.009});
      firstPlatformLevel.setRedundantJointEndPositions({
        -0.0302769856567722, -0.0664251770004387, 0.009,
        0.0726643835616438, -0.0069919497714374, 0.009,
        0.0726643835616438, 0.0069919497714374, 0.009,
        -0.0302769856567722, 0.0664251770004387, 0.009,
        -0.0423873979048716, 0.0594332272290012, 0.009,
        -0.0423873979048716,-0.0594332272290012, 0.009});

      platformLevels_.push_back(firstPlatformLevel);

      ParallelKinematicMachine6PUPS secondPlatformLevel;
      secondPlatformLevel.setMinimalActiveJointActuations({0.2148, 0.2148, 0.2148, 0.2148, 0.2148, 0.2148});
      secondPlatformLevel.setMaximalActiveJointActuations({0.3148, 0.3148, 0.3148, 0.3148, 0.3148, 0.3148});
      secondPlatformLevel.setEndEffectorJointPositions({
        0.0302769856567722, -0.0664251770004387, -0.009,
        0.0423873979048716, -0.0594332272290012, -0.009,
        0.0423873979048716, 0.0594332272290012, -0.009,
        0.0302769856567722, 0.0664251770004387, -0.009,
        -0.0726643835616438, 0.0069919497714374, -0.009,
        -0.0726643835616438, -0.0069919497714374, -0.009});
      secondPlatformLevel.setRedundantJointStartPositions({
        -0.0302769856567722, -0.0664251770004387, 0.009,
        0.0726643835616438, -0.0069919497714374, 0.009,
        0.0726643835616438, 0.0069919497714374, 0.009,
        -0.0302769856567722, 0.0664251770004387, 0.009,
        -0.0423873979048716, 0.0594332272290012, 0.009,
        -0.0423873979048716,-0.0594332272290012, 0.009});
      secondPlatformLevel.setRedundantJointEndPositions({
        -0.0302769856567722, -0.0664251770004387, 0.009,
        0.0726643835616438, -0.0069919497714374, 0.009,
        0.0726643835616438, 0.0069919497714374, 0.009,
        -0.0302769856567722, 0.0664251770004387, 0.009,
        -0.0423873979048716, 0.0594332272290012, 0.009,
        -0.0423873979048716,-0.0594332272290012, 0.009});

      platformLevels_.push_back(secondPlatformLevel);

      ParallelKinematicMachine6PUPS thirdPlatformLevel;
      thirdPlatformLevel.setMinimalActiveJointActuations({0.2148, 0.2148, 0.2148, 0.2148, 0.2148, 0.2148});
      thirdPlatformLevel.setMaximalActiveJointActuations({0.3148, 0.3148, 0.3148, 0.3148, 0.3148, 0.3148});
      thirdPlatformLevel.setEndEffectorJointPositions({
        0.0302769856567722, -0.0664251770004387, -0.009,
        0.0423873979048716, -0.0594332272290012, -0.009,
        0.0423873979048716, 0.0594332272290012, -0.009,
        0.0302769856567722, 0.0664251770004387, -0.009,
        -0.0726643835616438, 0.0069919497714374, -0.009,
        -0.0726643835616438, -0.0069919497714374, -0.009});
      thirdPlatformLevel.setRedundantJointStartPositions({
        -0.0302769856567722, -0.0664251770004387, 0.009,
        0.0726643835616438, -0.0069919497714374, 0.009,
        0.0726643835616438, 0.0069919497714374, 0.009,
        -0.0302769856567722, 0.0664251770004387, 0.009,
        -0.0423873979048716, 0.0594332272290012, 0.009,
        -0.0423873979048716,-0.0594332272290012, 0.009});
      thirdPlatformLevel.setRedundantJointEndPositions({
        -0.0302769856567722, -0.0664251770004387, 0.009,
        0.0726643835616438, -0.0069919497714374, 0.009,
        0.0726643835616438, 0.0069919497714374, 0.009,
        -0.0302769856567722, 0.0664251770004387, 0.009,
        -0.0423873979048716, 0.0594332272290012, 0.009,
        -0.0423873979048716,-0.0594332272290012, 0.009});

      platformLevels_.push_back(thirdPlatformLevel);
    }

    inline void MultiLevelStewartPlatform::setPlatformLevels(
        const std::vector<ParallelKinematicMachine6PUPS> platformLevels) noexcept {
      platformLevels_ = platformLevels;
    }

    inline std::vector<arma::Cube<double>::fixed<3, 6, 2>> MultiLevelStewartPlatform::getModel(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      std::vector<arma::Cube<double>::fixed<3, 6, 2>> models;

      models.push_back(platformLevels_.at(0).getModel(endEffectorPose, {}));

      for (std::size_t n = 1; n < platformLevels_.size(); ++n) {
        models.push_back(platformLevels_.at(n).getModel(redundantJointActuations.col(n), {}));
      }

      return models;
    }

    inline arma::Mat<double> MultiLevelStewartPlatform::getActuation(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const {
      arma::Mat<double> actuations;

      const arma::Mat<double>& actuation = platformLevels_.at(0).getActuation(endEffectorPose, {});
      actuations = arma::join_rows(actuations, actuation);

      for (std::size_t n = 1; n < platformLevels_.size(); ++n) {
        const arma::Mat<double>& actuation = platformLevels_.at(n).getActuation(redundantJointActuations.col(n), {});
        actuations = arma::join_rows(actuations, actuation);
      }

      return actuations;
    }

    inline double MultiLevelStewartPlatform::getEndEffectorPoseError(
        const arma::Col<double>::fixed<6>& endEffectorPose,
        const arma::Mat<double>& redundantActuationParameters) const {
      double positionError = platformLevels_.at(0).getEndEffectorPoseError(endEffectorPose, {});

      for (std::size_t n = 1; n < platformLevels_.size(); ++n) {
        positionError += platformLevels_.at(n).getEndEffectorPoseError(redundantActuationParameters.col(n), {});
      }

      return positionError;
    }
  }
}
