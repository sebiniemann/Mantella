namespace mant {
  namespace robotic {
    class KinematicMachine {
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

        virtual ~KinematicMachine() = default;
    };

    // TODO Add Opt. implementation
  }
}
