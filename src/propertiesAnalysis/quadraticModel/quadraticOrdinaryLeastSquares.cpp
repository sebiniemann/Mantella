#include <hop_bits/propertiesAnalysis/quadraticModel/quadraticOrdinaryLeastSquares.hpp>

// C++ Standard Library
#include <cstdlib>

// HOP
#include <hop_bits/helper/regression.hpp>

namespace hop {
  void QuadraticOrdinaryLeastSquares::analyseImplementation(
      const arma::Mat<double>& parameters,
      const arma::Col<double>& objectiveValues) {
    arma::Mat<double> quadraticModelParameters(parameters.n_rows * (parameters.n_rows + 3) / 2, parameters.n_cols);
    for (std::size_t n = 0; n < quadraticModelParameters.n_cols; ++n) {
      arma::Col<double> parameter = parameters.col(n);
      arma::Col<double> quadraticModelParameter(quadraticModelParameters.n_rows);

      quadraticModelParameter.subvec(0, parameter.n_elem - 1) = parameter;
      quadraticModelParameter.subvec(parameter.n_elem, 2 * parameter.n_elem - 1) = arma::square(parameter);

      std::size_t k = 2 * parameter.n_elem;
      for (std::size_t l = 0; l < parameter.n_elem; ++l) {
        for (std::size_t m = l + 1; m < parameter.n_elem; ++m) {
          quadraticModelParameter.at(k++) = parameter.at(l) * parameter.at(m);
        }
      }

      quadraticModelParameters.col(n) = quadraticModelParameter;
    }

    quadraticEstimator_ = getEstimator(quadraticModelParameters, objectiveValues);
  }
}
