namespace mant {
  namespace robotic {
    template <class Model>
    class EndEffectorPoseError : public OptimisationProblem<double> {
        explicit EndEffectorPoseError(const Model& model) noexcept;

        void setEndEffectorTrajectory(
            const arma::Mat<double> endEffectorTrajectory) noexcept;

      protected:
        Model model_;

        arma::Mat<double> endEffectorTrajectory_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;
    };

    template <class Model>
    EndEffectorPoseError<Model>::EndEffectorPoseError(
        const Model& model) noexcept
      : model_(model) {
      setEndEffectorTrajectory();
    }

    template <class Model>
    void EndEffectorPoseError<Model>::setEndEffectorTrajectory(
        const arma::Mat<double> endEffectorTrajectory) noexcept {
      endEffectorTrajectory_ = endEffectorTrajectory;
    }

    template <>
    inline double EndEffectorPoseError<ParallelKinematicMachine3PRRR>::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double endEffectorPoseError = 0.0;

      for(std::size_t n = 0; n < endEffectorTrajectory_.n_cols; ++n) {
        endEffectorPoseError = std::max(endEffectorPoseError, model_.getEndEffectorPoseError(endEffectorTrajectory_.col(n), parameter));
      }

      return endEffectorPoseError;
    }
  }
}
