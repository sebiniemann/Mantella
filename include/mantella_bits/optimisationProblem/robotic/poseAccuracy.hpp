namespace mant {
  namespace robotic {
    template <class Model>
    class PoseAccuracy : public OptimisationProblem<double> {
      protected:
        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;
    };

    template <>
    inline double PoseAccuracy<ParallelKinematicMachine3PRRR>::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return 0.0;
    }
  }
}
