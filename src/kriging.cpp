#include "mantella_bits/kriging.hpp"

// C++ standard library
#include <cassert>
#include <utility>

// Mantella
#include "mantella_bits/algebra.hpp"
#include "mantella_bits/armadillo.hpp"
#include "mantella_bits/assert.hpp"
#include "mantella_bits/statistics.hpp"

namespace mant {
  Kriging::Kriging(
      const std::function<double(const arma::vec&)> correlationFunction)
      : correlationFunction_(correlationFunction),
        highestDegree_(1) {}

  void Kriging::setCorrelationFunction(
      const std::function<double(const arma::vec&)> correlationFunction) {
    assert(correlationFunction && "Kriging.setCorrelationFunction: The correlation function must not be empty");
    correlationFunction_ = correlationFunction;
    trained_ = false;
  }

  const std::function<double(const arma::vec&)> Kriging::getCorrelationFunction() const {
    return correlationFunction_;
  }

  void Kriging::setHighestDegree(
      arma::uword highestDegree) {
    assert(highestDegree >= 0 && "Kriging.setHighestDegree: The highest degree of a Krigings polynomial function must not be negative");
    assert(highestDegree <= 2 && "Kriging.setHighestDegree: The highest supported degree for a Kriging polynomial function is 2");
    highestDegree_ = highestDegree;
    trained_ = false;
  }

  arma::uword Kriging::getHighestDegree() const {
    return highestDegree_;
  }

  void Kriging::train(
      const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples) {
    {
      arma::uword sampleDimension = samples.begin()->first.n_elem;
      for (auto& sample : samples) {
        assert(sample.n_elem == sampleDimension && "Kriging: the dimension of all samples must be consistent.");
      }
      trained_ = false;
      samples_ = samples;
    }

    arma::mat parameters(samples_.begin()->first.n_elem, samples_.size());
    arma::rowvec objectiveValues(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      parameters.col(n) = sample.first;
      objectiveValues(n) = sample.second;
      ++n;
    }

    meanParameter_ = arma::mean(parameters, 1);
    standardDeviationParameter_ = arma::stddev(parameters, 0, 1);

    parameters.each_col() -= meanParameter_;
    parameters.each_col() /= standardDeviationParameter_;

    meanObjectiveValue_ = arma::mean(objectiveValues);
    standardDeviationObjectiveValue_ = arma::stddev(objectiveValues);

    objectiveValues -= meanObjectiveValue_;
    objectiveValues /= standardDeviationObjectiveValue_;

    arma::mat correlations(parameters.n_cols, parameters.n_cols);
    correlations.diag().zeros();

    arma::mat polynomials(polynomialSize(parameters.n_rows, highestDegree_), parameters.n_cols);
    for (n = 0; n < parameters.n_cols; ++n) {
      const arma::vec& parameter = parameters.col(n);
      for (arma::uword k = n + 1; k < parameters.n_cols; ++k) {
        correlations(n, k) = correlationFunction_(parameters.col(k) - parameter);
      }

      polynomials.col(n) = polynomialFunction(parameter);
    }
    correlations = arma::symmatu(correlations);

    beta_ = generalisedLeastSquaresEstimate(polynomials, objectiveValues, correlations);
    gamma_ = arma::solve(correlations, objectiveValues.t() - polynomials * beta_);

    trained_ = true;
  }

  double Kriging::predict(
      const arma::vec& parameter) const {
    assert(trained_ && "Kriging.predict: Either this Kriging has never been trained on any data, or the correlation function or polynomial degree was changed since the last training. Please call `Kriging.train` first.");
    assert(parameters.n_elem == meanParameter_.n_elem && "Kriging.predict: the dimension of the predicted value must match that of the training data.");

    const arma::vec& standardisedParameter = (parameter - meanParameter_) / standardDeviationParameter_;

    arma::vec correlations(samples_.size());
    arma::uword n = 0;
    for (const auto& sample : samples_) {
      correlations(n++) = correlationFunction_(sample.first - standardisedParameter);
    }

    return meanObjectiveValue_ + (arma::dot(beta_, polynomialFunction(parameter)) + arma::dot(gamma_, correlations)) * standardDeviationObjectiveValue_;
  }


  /**
   * TODO: Move this to the documentation area, where users have chance to
   * find it
   *
   * The result depends on the regression model:
   *
   * ##constant (highestDegree_ = 0)
   *
   *     p: R^n -> R^n,
   *     p(x1, ..., xn) = (1, ..., 1)
   *
   * ##linear (highestDegree_ = 1)
   *
   *     p: R^n -> R^{n+1},
   *     p(x1, ..., xn) = (1, x1, ..., xn)
   *
   * ##quadratic (highestDegree_ = 2)
   *
   *     p: R^n -> R^{(n+1)(n+2)/2},
   *     p(x1, ..., xn) = (1, x1, ..., xn, x1*x1, ..., x1*xn, ... xn*x1, x2*x2, ..., x2*xn, ... xn*xn)
   */
  arma::vec Kriging::polynomialFunction(
      const arma::vec& parameter) const {
    switch(highestDegree_) {
      case 0:
        return arma::vec(parameter.n_elem, 1);
      case 1: {
        arma::vec result(parameter.n_elem + 1);
        result(1) = 1;
        for (int i = 0; i < parameter.n_elem; i++) {
          result(i + 1) = parameter(i);
        }
        return result;
      }
      //case 2:
      default:
        return arma::vec();
    }
  }
}
