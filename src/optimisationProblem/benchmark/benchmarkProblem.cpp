#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

#include <algorithm>
using std::min;
using std::max;

#include <cmath>
using std::pow;
using std::log;
using std::abs;
using std::sin;
using std::sqrt;
using std::exp;
using std::copysign;

#include <random>
using std::cauchy_distribution;

#include <armadillo>
using arma::zeros;
using arma::randu;
using arma::randn;
using arma::norm;
using arma::linspace;

#include <helper/random.hpp>

namespace hop {
  BenchmarkProblem::BenchmarkProblem(const unsigned int& numberOfDimensions) : OptimisationProblem(numberOfDimensions), _randomTranslation(randu(_numberOfDimensions) * 8 - 4) {
    _lowerBounds.fill(-5.0);
    _upperBounds.fill(5.0);

    _objectiveValueTranslation = min(1000.0, max(-1000.0, cauchy_distribution<double>(0.0, 100.0)(Random::RNG)));
  }

  Col<double> BenchmarkProblem::getRandomParameterTranslation(const Col<double>& parameter) const {
    return parameter - _randomTranslation;
  }

  Mat<double> BenchmarkProblem::getRandomRotation() const {
    Mat<double> rotationMatrix = randn(_numberOfDimensions, _numberOfDimensions);
    for (int j = 0; j < rotationMatrix.n_cols; j++) {
      Col<double> colJ = rotationMatrix.col(j);
      for (int jj = 0; jj < j-1; jj++) {
        Col<double> colJJ = rotationMatrix.col(jj);

        rotationMatrix.col(j) = colJ - colJ.t() * colJJ * colJJ;
      }
      rotationMatrix.col(j) = colJ / norm(colJ);
    }

    return rotationMatrix;
  }

  Col<double> BenchmarkProblem::getScaling(const double& condition) const {
    Col<double> scaling = linspace(0, 1, _numberOfDimensions);

    for (auto& scale : scaling) {
      scale = pow(condition, scale);
    }

    return scaling;
  }

  Col<double> BenchmarkProblem::getScaling(const Col<double>& condition) const {
    Col<double> scaling = linspace(0, 1, _numberOfDimensions);

    for (size_t n = 0; n < scaling.n_elem; n++) {
      scaling.at(n) = pow(condition.at(n), scaling.at(n));
    }

    return scaling;
  }

  Col<double> BenchmarkProblem::getAsymmetricTransformation(const double& beta, const Col<double>& parameter) const {
    Col<double> asymmetricTransformation(parameter.n_elem);
    Col<double> spacing = linspace(0, 1, _numberOfDimensions);

    for (size_t n = 0; n < parameter.n_elem; n++) {
      double value = parameter.at(n);

      if (value > 0.0) {
        asymmetricTransformation.at(n) = pow(value, 1 + beta * spacing.at(n) * sqrt(value));
      } else {
        asymmetricTransformation.at(n) = value;
      }
    }

    return asymmetricTransformation;
  }

  double BenchmarkProblem::getOscillationTransformation(const double& value) const {
    if(value != 0.0) {
      double c1;
      double c2;
      if (value > 0.0) {
        c1 = 10.0;
        c2 = 7.9;
      } else {
        c1 = 5.5;
        c2 = 3.1;
      }

      double x = log(abs(value));
      return copysign(1.0, value) * std::exp(x + 0.049 * (sin(c1 * x) + sin(c2 * x)));
    } else {
      return 0.0;
    }
  }

  Col<double> BenchmarkProblem::getOscillationTransformation(const Col<double>& parameter) const {
    Col<double> oscillate(parameter.n_elem);

    for (size_t n = 0; n < parameter.n_elem; n++) {
      oscillate.at(n) = getOscillationTransformation(parameter.at(n));
    }

    return oscillate;
  }

  double BenchmarkProblem::getPenality(const Col<double>& parameter) const {
    double penality = 0.0;

    for (size_t n = 0; n < parameter.n_elem; n++) {
      penality += pow(max(0.0, abs(parameter.at(n)) - 5.0), 2.0);
    }

    return penality;
  }
}
