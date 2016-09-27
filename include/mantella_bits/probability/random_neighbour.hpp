#pragma once

/**

*/
  arma::vec randomNeighbour(
      const arma::vec& parameter,
      const double minimalDistance,
      const double maximalDistance) {
    assert(!parameter.is_empty() && "randomNeighbour: The parameter must be non-empty.");
    assert(!parameter.has_nan() && "randomNeighbour: The parameter must not have NaNs.");
    assert(minimalDistance >= 0 && "randomNeighbour: The minimal distance must be positive (including 0).");
    assert(minimalDistance <= maximalDistance && "randomNeighbour: The minimal distance must be less than or equal to the maximal one.");

    // @see J. S. Hicks and R. F. Wheeling (1959). An efficient method for generating uniformly distributed points on the surface of an n-dimensional sphere. Communications of the ACM, 2(4), pp. 17-19.
    return parameter + arma::normalise(normalRandomNumbers(parameter.n_rows, parameter.n_cols)) * (minimalDistance + std::uniform_real_distribution<double>(0, 1)(Rng::getGenerator()) * (maximalDistance - minimalDistance));
  }

  arma::vec randomNeighbour(
      const arma::vec& parameter,
      const double maximalDistance) {
    return randomNeighbour(parameter, 0.0, maximalDistance);
  }