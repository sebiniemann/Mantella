namespace mant {
  class InfinityNorm : public DistanceFunction<double> {
    public:
      using DistanceFunction<double>::DistanceFunction;

    protected:
      inline double getDistanceImplementation(
          const arma::Col<double>& parameter) const noexcept override;

      inline arma::Col<double> getRandomNeighbourImplementation(
          const arma::Col<double>& parameter,
          const double& minimalDistance,
          const double& maximalDistance) const noexcept override;
  };

  //
  // Implementation
  //

  inline double InfinityNorm::getDistanceImplementation(
      const arma::Col<double>& parameter) const noexcept {
    return arma::norm(parameter, "inf");
  }

  inline arma::Col<double> InfinityNorm::getRandomNeighbourImplementation(
      const arma::Col<double>& parameter,
      const double& minimalDistance,
      const double& maximalDistance) const noexcept {
    assert(minimalDistance <= maximalDistance);

    const arma::Col<double>& velocity = 2.0 * arma::randu<arma::Col<double>>(parameter.n_elem) - 1.0;
    return parameter + arma::sign(velocity) * minimalDistance + (maximalDistance - minimalDistance) * velocity;
  }
}
