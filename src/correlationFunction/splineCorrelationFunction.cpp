#include <mantella_bits/correlationFunction/splineCorrelationFunction.hpp>

namespace mant {
  double SplineCorrelationFunction::getCorrelationCoefficientImplementation(
      const arma::Col<double>& parameter) const {
    arma::Col<double> aboluteParameter = arma::abs(parameter);

    if (arma::any(parameter >= 1)) {
      return 0;
    } else {
      const arma::Col<arma::uword>& firstPart = arma::find(aboluteParameter <= 0.2);
      const arma::Col<arma::uword>& secondPart = arma::find(0.2 < aboluteParameter && aboluteParameter < 1);

      aboluteParameter.elem(firstPart) = 1 - arma::pow(aboluteParameter.elem(firstPart), 2.0) % (15 - 30 * aboluteParameter.elem(firstPart));
      aboluteParameter.elem(secondPart) = 1.25 * arma::pow(1.0 - aboluteParameter.elem(secondPart), 3.0);

      return arma::prod(aboluteParameter);
    }
  }

  std::string SplineCorrelationFunction::toString() const {
    return "spline_correlation_function";
  }
}
