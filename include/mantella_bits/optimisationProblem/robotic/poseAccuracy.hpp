namespace mant {
  namespace robotic {
    template <class Model>
    class PoseAccuracy : public OptimisationProblem<double> {
        explicit PoseAccuracy(const Model& model) noexcept;

        void setEndEffectorTrajectory(
            const arma::Mat<double> endEffectorTrajectory) noexcept;

      protected:
        Model model_;

        arma::Mat<double> endEffectorTrajectory_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;
    };

    template <class Model>
    PoseAccuracy<Model>::PoseAccuracy(
        const Model& model) noexcept
      : model_(model) {
      setEndEffectorTrajectory();
    }

    template <class Model>
    void PoseAccuracy<Model>::setEndEffectorTrajectory(
        const arma::Mat<double> endEffectorTrajectory) noexcept {
      endEffectorTrajectory_ = endEffectorTrajectory;
    }

    template <>
    inline double PoseAccuracy<ParallelKinematicMachine3PRRR>::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double poseAccuracy = 0.0;

      for(std::size_t n = 0; n < endEffectorTrajectory_.n_cols; ++n) {
        poseAccuracy = std::max(poseAccuracy, model_.getEndEffectorPoseAccuracy(endEffectorTrajectory_.col(n), parameter));
      }

      return poseAccuracy;
    }
  }
}
