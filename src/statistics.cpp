#include "mantella_bits/statistics.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  double medianAbsoluteError(
      const arma::Row<double>& data) {
    return median(arma::abs(data - median(data)));
  }

  double percentile(
      const arma::Row<double>& data,
      const double nthPercentile) {
    verify(0.0 < nthPercentile && nthPercentile <= 100.0, "percentile: The percentile to be computed must be within (0, 100].");

    const arma::Row<double>& sortedData = arma::sort(data, "descend");
    // Gets the precise index of the *nthPercentile*.
    double index = nthPercentile / 100.0 * static_cast<double>(sortedData.n_elem - 1);

    arma::uword lowerIndex = static_cast<arma::uword>(std::floor(index));
    arma::uword upperIndex = static_cast<arma::uword>(std::ceil(index));

    // If the *lowerIndex* is equal to the *upperIndex*, simply return the value at this index. Otherwise, interpolate between both indices, weighting both elements according to the distance towards the index.
    // For example: Given an index of 2.3, the result should be sortedData(2) * 0.7 + sortedData(3) + 0.3.
    if (lowerIndex != upperIndex) {
      // Inverses the weight. Otherwise the index with the larger distance would get the greater weight, instead of the nearest index.
      return (static_cast<double>(upperIndex) - index) * sortedData(lowerIndex) + (index - static_cast<double>(lowerIndex)) * sortedData(upperIndex);
    } else {
      return sortedData(upperIndex);
    }
  }

  double decile(
      const arma::Row<double>& data,
      const double nthDecile) {
    verify(0.0 < nthDecile && nthDecile <= 10.0, "decile: The decile must be within (0, 10].");

    return percentile(data, nthDecile * 10.0);
  }

  double quartile(
      const arma::Row<double>& data,
      const double nthQuartile) {
    verify(0.0 < nthQuartile && nthQuartile <= 4.0, "quartile: The decile must be within (0, 4].");

    return percentile(data, nthQuartile * 25.0);
  }

  arma::Col<double> ordinaryLeastSquaresEstimate(
      const arma::Mat<double>& parameters,
      const arma::Row<double>& objectiveValues) {
    verify(parameters.n_cols == objectiveValues.n_elem, "ordinaryLeastSquaresEstimate: The number of parameters must be equal to the number of objective values.");

    return arma::solve(parameters * parameters.t(), parameters) * objectiveValues.t();
  }

  arma::Col<double> generalisedLeastSquaresEstimate(
      const arma::Mat<double>& parameters,
      const arma::Row<double>& objectiveValues,
      const arma::Mat<double>& covariance) {
    verify(parameters.n_cols == objectiveValues.n_elem, "generalisedLeastSquaresEstimate: The number of parameters must be equal to the number of objective values.");
    verify(parameters.n_cols == covariance.n_rows, "generalisedLeastSquaresEstimate: The number of rows of the covariance matrix must be equal to number of parameters.");
    verify(isPositiveSemiDefinite(covariance), "generalisedLeastSquaresEstimate: The covariance matrix must be positive semi-definite.");

    arma::Mat<double> cholesky = arma::chol(covariance);

    // Asserts that *cholesky* is an upper triangular matrix.
    assert(arma::all(arma::vectorise(cholesky == arma::trimatu(cholesky))));

    return ordinaryLeastSquaresEstimate(parameters * cholesky, objectiveValues * cholesky);
  }
}
