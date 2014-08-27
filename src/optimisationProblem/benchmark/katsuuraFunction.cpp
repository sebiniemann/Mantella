#include <optimisationProblem/benchmark/katsuuraFunction.hpp>

#include <cmath>

namespace hop {
  KatsuuraFunction::KatsuuraFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(std::sqrt(100.0))) {

  }

  double KatsuuraFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = _rotationQ * (_delta % (_rotationR * (parameter - _translation)));

    double product = 1.0;
    for (std::size_t n = 0; n < z.n_elem; n++) {
        double value = z.at(n);

        double sum = 0.0;
        for (unsigned int k = 1; k < 33; k++) {
            double power = std::pow(2.0, k);
            sum += std::abs(power * value - std::round(power * value)) / power;
        }

        product *= std::pow(1.0 + static_cast<double>(n + 1) * sum, (10.0 / std::pow(_numberOfDimensions, 1.2)));
    }

    return 10.0 / std::pow(_numberOfDimensions, 2) * (product - 1.0) + getPenality(parameter);
  }
}
