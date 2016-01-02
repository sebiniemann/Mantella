#include "mantella_bits/optimisationProblem/surrogateModel/krigingModel.hpp"

// C++ standard library
#include <unordered_map>
#include <utility>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual
#include "mantella_bits/statistics.hpp"

namespace mant {
  KrigingModel::KrigingModel(
      const arma::uword numberOfDimensions)
      : SurrogateModel(numberOfDimensions) {
    setModelFunction([this](
        const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples) {
        arma::Mat<double> parameters(numberOfDimensions_, samples.size());
        arma::Row<double> objectiveValues(samples.size());
        arma::uword n = 0;
        for (const auto& sample : samples) {
          parameters.col(n) = sample.first;
          objectiveValues(n) = sample.second;
          ++n;
        }

        meanParameter_ = arma::mean(parameters, 2);
        standardDeviationParameter_ = arma::stddev(parameters, 2);

        parameters.each_col() -= meanParameter_;
        parameters.each_col() /= standardDeviationParameter_;

        meanObjectiveValue_ = arma::mean(objectiveValues);
        standardDeviationObjectiveValue_ = arma::stddev(objectiveValues);

        objectiveValues -= meanObjectiveValue_;
        objectiveValues /= standardDeviationObjectiveValue_;

        arma::Mat<double> correlations(parameters.n_cols, parameters.n_cols);
        correlations.diag().zeros();

        for (n = 0; n < parameters.n_cols; ++n) {
          const arma::Col<double>& parameter = parameters.col(n);
          for (arma::uword k = n + 1; k < parameters.n_cols; ++k) {
            correlations(n, k) = correlationFunction_(parameters.col(k) - parameter);
          }

          parameters.col(n) = regressionFunction_(parameter);
        }
        correlations = arma::symmatu(correlations);

        beta_ = generalisedLeastSquaresEstimate(parameters, objectiveValues, correlations);
        gamma_ = arma::solve(correlations, objectiveValues.t() - parameters * beta_);
    
        return [this] (
                        const arma::Col<double>& parameter) {
          const arma::Col<double>& normalisedParameter = (parameter - meanParameter_) / standardDeviationParameter_;

          arma::Col<double> correlations(samples.size());
          arma::uword n = 0;
          for (const auto& sample : samples) {
            correlations(n++) = correlationFunction_(sample.first - normalisedParameter);
          }

          return meanObjectiveValue_ + (arma::dot(beta_, regressionFunction_(parameter)) + arma::dot(gamma_, correlations)) * standardDeviationObjectiveValue_;
        };
    });
  }
}
