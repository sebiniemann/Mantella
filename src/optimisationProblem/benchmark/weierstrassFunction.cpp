#include <optimisationProblem/benchmark/weierstrassFunction.hpp>

#include <cmath>
using std::sqrt;
using std::pow;
using std::cos;

#include <armadillo>
using arma::datum;

namespace hop {
  WeierstrassFunction::WeierstrassFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(sqrt(0.01))), _rotationR(getRandomRotation()), _rotationQ(getRandomRotation()) {
    _f0 = 0.0;
    for(unsigned int k = 0; k < 12; k++) {
      _f0 += pow(0.5, k) * cos(2.0 * datum::pi * pow(3, k) * 0.5);
    }
  }

  double WeierstrassFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = _rotationR * diagmat(_delta) * _rotationQ * getOscillationTransformation(_rotationR * getRandomParameterTranslation(parameter));

    double sum = 0;
    for (size_t n = 0; n < parameter.n_elem; n++) {
      for (unsigned int k = 0; k < 12; k++) {
        sum += pow(0.5, k) * cos(2.0 * datum::pi * pow(3.0, k) * (z.at(n) + 0.5));
      }
    }

    return 10 * (pow(sum / static_cast<double>(_numberOfDimensions) - _f0, 3) + getPenality(parameter) / static_cast<double>(_numberOfDimensions));
  }
}
