namespace mant {
  namespace robotic {
    class ParallelKinematicMachine {
      public:
        virtual std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const = 0;

        virtual arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const = 0;

        virtual double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const = 0;

        virtual ~ParallelKinematicMachine() = default;
    };

    // TODO Add Opt. implementation
  }
}
