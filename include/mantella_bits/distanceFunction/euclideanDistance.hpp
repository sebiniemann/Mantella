namespace mant {
  class EuclideanDistance : public DistanceFunction<double> {
    public:
      using DistanceFunction<double>::DistanceFunction;

    protected:
      inline double getDistanceImplementation(
          const arma::Col<double>& parameter) const noexcept override;

      inline arma::Col<double> getNeighbourImplementation(
          const arma::Col<double>& parameter,
          const double& minimalDistance,
          const double& maximalDistance) const noexcept override;
  };

  //
  // Implementation
  //

  inline double EuclideanDistance::getDistanceImplementation(
      const arma::Col<double>& parameter) const noexcept {
    return arma::norm(parameter);
  }

  inline arma::Col<double> EuclideanDistance::getNeighbourImplementation(
      const arma::Col<double>& parameter,
      const double& minimalDistance,
      const double& maximalDistance) const noexcept {
    const arma::Col<double>& velocity = arma::normalise(arma::randn<arma::Col<double>>(parameter.n_elem)) * std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
    return parameter + arma::sign(velocity) * minimalDistance + (maximalDistance - minimalDistance) * velocity;
  }
}
