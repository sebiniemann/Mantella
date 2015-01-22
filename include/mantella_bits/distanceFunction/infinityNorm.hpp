namespace mant {
  class InfinityNorm : public DistanceFunction<double> {
    protected:
      inline double getDistanceImplementation(
          const arma::Col<double>& parameter) const  override;

      inline arma::Col<double> getNeighbourImplementation(
          const arma::Col<double>& parameter,
          const double& minimalDistance,
          const double& maximalDistance) const  override;
  };

  //
  // Implementation
  //

  inline double InfinityNorm::getDistanceImplementation(
      const arma::Col<double>& parameter) const  {
    return arma::norm(parameter, "inf");
  }

  inline arma::Col<double> InfinityNorm::getNeighbourImplementation(
      const arma::Col<double>& parameter,
      const double& minimalDistance,
      const double& maximalDistance) const  {
    const arma::Col<double>& velocity = 2.0 * arma::randu<arma::Col<double>>(parameter.n_elem) - 1.0;
    return parameter + arma::sign(velocity) * minimalDistance + (maximalDistance - minimalDistance) * velocity;
  }
}
