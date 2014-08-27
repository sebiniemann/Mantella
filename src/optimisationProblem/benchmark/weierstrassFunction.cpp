#include <optimisationProblem/benchmark/weierstrassFunction.hpp>

#include <cmath>

namespace hop {
  WeierstrassFunction::WeierstrassFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(0.01))) {
    _f0 = 0.0;
    for(unsigned int k = 0; k < 12; k++) {
      _f0 += std::pow(0.5, k) * cos(2.0 * arma::datum::pi * pow(3, k) * 0.5);
    }
  }

  double WeierstrassFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = _rotationR * (_delta % (_rotationQ * getOscillationTransformation(_rotationR * (parameter - _translation))));

    double sum = 0;
    for (std::size_t n = 0; n < parameter.n_elem; n++) {
      for (unsigned int k = 0; k < 12; k++) {
        sum += std::pow(0.5, k) * std::cos(2.0 * arma::datum::pi * std::pow(3.0, k) * (z.at(n) + 0.5));
      }
    }

    return 10 * (std::pow(sum / static_cast<double>(_numberOfDimensions) - _f0, 3) + getPenality(parameter) / static_cast<double>(_numberOfDimensions));
  }
}
