namespace mant {
  namespace robotic {
    class MultiLevelStewartPlatform : public ParallelKinematicMachine {
      public:
        inline explicit MultiLevelStewartPlatform() noexcept;

        inline explicit MultiLevelStewartPlatform(
            const std::vector<ParallelKinematicMachine6PUPS>& platformLevels) noexcept;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        MultiLevelStewartPlatform(const MultiLevelStewartPlatform&) = delete;
        MultiLevelStewartPlatform& operator=(const MultiLevelStewartPlatform&) = delete;

        inline std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        inline arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        inline double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

      protected:
        const std::vector<ParallelKinematicMachine6PUPS> platformLevels_;
    };

    //
    // Implementation
    //

    inline MultiLevelStewartPlatform::MultiLevelStewartPlatform() noexcept
      : platformLevels_({
        ParallelKinematicMachine6PUPS({
          0.0302769856567722, -0.0664251770004387, -0.009,
          0.0423873979048716, -0.0594332272290012, -0.009,
          0.0423873979048716, 0.0594332272290012, -0.009,
          0.0302769856567722, 0.0664251770004387, -0.009,
          -0.0726643835616438, 0.0069919497714374, -0.009,
          -0.0726643835616438, -0.0069919497714374, -0.009
        }, {
         -0.0302769856567722, -0.0664251770004387, 0.009,
         0.0726643835616438, -0.0069919497714374, 0.009,
         0.0726643835616438, 0.0069919497714374, 0.009,
         -0.0302769856567722, 0.0664251770004387, 0.009,
         -0.0423873979048716, 0.0594332272290012, 0.009,
         -0.0423873979048716,-0.0594332272290012, 0.009
       }, {
          -0.0302769856567722, -0.0664251770004387, 0.009,
          0.0726643835616438, -0.0069919497714374, 0.009,
          0.0726643835616438, 0.0069919497714374, 0.009,
          -0.0302769856567722, 0.0664251770004387, 0.009,
          -0.0423873979048716, 0.0594332272290012, 0.009,
          -0.0423873979048716,-0.0594332272290012, 0.009
        }, {
          0.2148, 0.2148, 0.2148, 0.2148, 0.2148, 0.2148
        }, {
          0.3148, 0.3148, 0.3148, 0.3148, 0.3148, 0.3148
        }),
        ParallelKinematicMachine6PUPS({
          0.0302769856567722, -0.0664251770004387, -0.009,
          0.0423873979048716, -0.0594332272290012, -0.009,
          0.0423873979048716, 0.0594332272290012, -0.009,
          0.0302769856567722, 0.0664251770004387, -0.009,
          -0.0726643835616438, 0.0069919497714374, -0.009,
          -0.0726643835616438, -0.0069919497714374, -0.009
        }, {
         -0.0302769856567722, -0.0664251770004387, 0.009,
         0.0726643835616438, -0.0069919497714374, 0.009,
         0.0726643835616438, 0.0069919497714374, 0.009,
         -0.0302769856567722, 0.0664251770004387, 0.009,
         -0.0423873979048716, 0.0594332272290012, 0.009,
         -0.0423873979048716,-0.0594332272290012, 0.009
       }, {
          -0.0302769856567722, -0.0664251770004387, 0.009,
          0.0726643835616438, -0.0069919497714374, 0.009,
          0.0726643835616438, 0.0069919497714374, 0.009,
          -0.0302769856567722, 0.0664251770004387, 0.009,
          -0.0423873979048716, 0.0594332272290012, 0.009,
          -0.0423873979048716,-0.0594332272290012, 0.009
        }, {
          0.2148, 0.2148, 0.2148, 0.2148, 0.2148, 0.2148
        }, {
          0.3148, 0.3148, 0.3148, 0.3148, 0.3148, 0.3148
        }),
        ParallelKinematicMachine6PUPS({
          0.0302769856567722, -0.0664251770004387, -0.009,
          0.0423873979048716, -0.0594332272290012, -0.009,
          0.0423873979048716, 0.0594332272290012, -0.009,
          0.0302769856567722, 0.0664251770004387, -0.009,
          -0.0726643835616438, 0.0069919497714374, -0.009,
          -0.0726643835616438, -0.0069919497714374, -0.009
        }, {
         -0.0302769856567722, -0.0664251770004387, 0.009,
         0.0726643835616438, -0.0069919497714374, 0.009,
         0.0726643835616438, 0.0069919497714374, 0.009,
         -0.0302769856567722, 0.0664251770004387, 0.009,
         -0.0423873979048716, 0.0594332272290012, 0.009,
         -0.0423873979048716,-0.0594332272290012, 0.009
       }, {
          -0.0302769856567722, -0.0664251770004387, 0.009,
          0.0726643835616438, -0.0069919497714374, 0.009,
          0.0726643835616438, 0.0069919497714374, 0.009,
          -0.0302769856567722, 0.0664251770004387, 0.009,
          -0.0423873979048716, 0.0594332272290012, 0.009,
          -0.0423873979048716,-0.0594332272290012, 0.009
        }, {
          0.2148, 0.2148, 0.2148, 0.2148, 0.2148, 0.2148
        }, {
          0.3148, 0.3148, 0.3148, 0.3148, 0.3148, 0.3148
        })
      }) {

    }

    inline MultiLevelStewartPlatform::MultiLevelStewartPlatform(
        const std::vector<ParallelKinematicMachine6PUPS>& platformLevels) noexcept
      : platformLevels_(platformLevels) {

    }

    inline std::vector<arma::Mat<double>> MultiLevelStewartPlatform::getModelCharacterisation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      std::vector<arma::Mat<double>> modelCharacterisations;

      const std::vector<arma::Mat<double>>& modelCharacterisation = platformLevels_.at(0).getModelCharacterisation(endEffectorPose, {});
      modelCharacterisations.insert(modelCharacterisations.end(), modelCharacterisation.begin(), modelCharacterisation.end());

      for (std::size_t n = 1; n < platformLevels_.size(); ++n) {
        const std::vector<arma::Mat<double>>& modelCharacterisation = platformLevels_.at(n).getModelCharacterisation(redundantJointActuations.col(n), {});
        modelCharacterisations.insert(modelCharacterisations.end(), modelCharacterisation.begin(), modelCharacterisation.end());
      }

      return modelCharacterisations;
    }

    inline arma::Mat<double> MultiLevelStewartPlatform::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantJointActuations) const noexcept {
      arma::Mat<double> actuations;

      const arma::Mat<double>& actuation = platformLevels_.at(0).getActuation(endEffectorPose, {});
      actuations = arma::join_rows(actuations, actuation);

      for (std::size_t n = 1; n < platformLevels_.size(); ++n) {
        const arma::Mat<double>& actuation = platformLevels_.at(n).getActuation(redundantJointActuations.col(n), {});
        actuations = arma::join_rows(actuations, actuation);
      }

      return actuations;
    }

    inline double MultiLevelStewartPlatform::getPositionError(
        const arma::Col<double>& endEffectorPose,
        const arma::Mat<double>& redundantActuationParameters) const noexcept {

      double positionError = platformLevels_.at(0).getPositionError(endEffectorPose, {});

      for (std::size_t n = 1; n < platformLevels_.size(); ++n) {
        positionError += platformLevels_.at(n).getPositionError(redundantActuationParameters.col(n), {});
      }

      return positionError;
    }
  }
}
