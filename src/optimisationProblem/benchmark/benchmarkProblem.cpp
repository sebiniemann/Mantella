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
using std::bernoulli_distribution;
using std::uniform_int_distribution;

#include <armadillo>
using arma::zeros;
using arma::randu;
using arma::randn;
using arma::norm;
using arma::linspace;
using arma::dot;

#include <helper/random.hpp>

namespace hop {
  BenchmarkProblem::BenchmarkProblem(const unsigned int& numberOfDimensions) : OptimisationProblem(numberOfDimensions) {
    setLowerBounds(zeros<Col<double>>(numberOfDimensions) - 5.0);
    setUpperBounds(zeros<Col<double>>(numberOfDimensions) + 5.0);
    setObjectiveValueTranslation(min(1000.0, max(-1000.0, cauchy_distribution<double>(0.0, 100.0)(Random::RNG))));

    setTranslation(randu(numberOfDimensions) * 8.0 - 4.0);
    setOne(zeros<Col<double>>(numberOfDimensions) + (bernoulli_distribution(0.5)(Random::RNG) ? 1.0 : -1.0));
    setRotationR(getRandomRotation());
    setRotationQ(getRandomRotation());
    setDeltaC101(getRandomDeltaC101());
    setLocalOptimaY101(getRandomLocalOptimaY101());
    setDeltaC21(getRandomDeltaC21());
    setLocalOptimaY21(getRandomLocalOptimaY21());
  }

  void BenchmarkProblem::setTranslation(const Col<double>& translation) {
    _translation = translation;
  }

  void BenchmarkProblem::setOne(const Col<double>& one) {
    _one = one;
  }

  void BenchmarkProblem::setRotationR(const Mat<double>& rotationR) {
    _rotationR = rotationR;
  }

  void BenchmarkProblem::setRotationQ(const Mat<double>& rotationQ) {
    _rotationQ = rotationQ;
  }

  void BenchmarkProblem::setDeltaC101(const Mat<double>& deltaC101) {
    _deltaC101 = deltaC101;
  }

  void BenchmarkProblem::setLocalOptimaY101(const Mat<double>& localOptimaY101) {
    _localOptimaY101 = localOptimaY101;
  }

  void BenchmarkProblem::setDeltaC21(const Mat<double>& deltaC21) {
    _deltaC21 = deltaC21;
  }

  void BenchmarkProblem::setLocalOptimaY21(const Mat<double>& localOptimaY21) {
    _localOptimaY21 = localOptimaY21;
  }

  Mat<double> BenchmarkProblem::getRandomRotation() const {
    Mat<double> rotationMatrix = randn(_numberOfDimensions, _numberOfDimensions);
    for (unsigned int j = 0; j < rotationMatrix.n_cols; j++) {
      for (unsigned int jj = 0; jj < j; jj++) {
        rotationMatrix.col(j) = rotationMatrix.col(j) - dot(rotationMatrix.col(j), rotationMatrix.col(jj)) * rotationMatrix.col(jj);
      }
      rotationMatrix.col(j) = rotationMatrix.col(j) / norm(rotationMatrix.col(j));
    }

    return rotationMatrix;
  }

  Mat<double> BenchmarkProblem::getRandomDeltaC101() const {
    Mat<double> deltaC101(_numberOfDimensions, 101);
    deltaC101.col(0) = getScaling(sqrt(1000.0)) / pow(1000.0, 0.25);

    uniform_int_distribution<int> uniformIntDistribution(0, 99);
    for(size_t j = 1; j < deltaC101.n_cols; j++) {
      deltaC101.col(j) = getScaling(sqrt(1000.0)) / pow(pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Random::RNG)) / 99.0), 0.25);
    }

    return deltaC101;
  }

  Mat<double> BenchmarkProblem::getRandomDeltaC21() const {
    Mat<double> deltaC21(_numberOfDimensions, 21);
    deltaC21.col(0) = getScaling(1000.0) / pow(1000.0, 0.25);

    uniform_int_distribution<int> uniformIntDistribution(0, 19);
    for(size_t j = 1; j < deltaC21.n_cols; j++) {
      deltaC21.col(j) = getScaling(sqrt(1000.0)) / pow(pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Random::RNG)) / 19.0), 0.25);
    }

    return deltaC21;
  }

  Mat<double> BenchmarkProblem::getRandomLocalOptimaY101() const {
    Mat<double> localOptimaY101 = randu(_numberOfDimensions, 101) * 8.0 - 4.0;
    localOptimaY101.col(0) = 0.8 * localOptimaY101.col(0);

    return localOptimaY101;
  }

  Mat<double> BenchmarkProblem::getRandomLocalOptimaY21() const {
    Mat<double> localOptimaY21 = randu(_numberOfDimensions, 21) * 9.8 - 4.9;
    localOptimaY21.col(0) = 0.8 * localOptimaY21.col(0);

    return localOptimaY21;
  }

  Col<double> BenchmarkProblem::getScaling(const double& condition) const {
    Col<double> scaling = linspace<Col<double>>(0, 1, _numberOfDimensions);

    for (auto& scale : scaling) {
      scale = pow(condition, scale);
    }

    return scaling;
  }

  Col<double> BenchmarkProblem::getScaling(const Col<double>& condition) const {
    Col<double> scaling = linspace<Col<double>>(0, 1, _numberOfDimensions);

    for (size_t n = 0; n < scaling.n_elem; n++) {
      scaling.at(n) = pow(condition.at(n), scaling.at(n));
    }

    return scaling;
  }

  Col<double> BenchmarkProblem::getAsymmetricTransformation(const double& beta, const Col<double>& parameter) const {
    Col<double> asymmetricTransformation(parameter.n_elem);
    Col<double> spacing = linspace<Col<double>>(0, 1, _numberOfDimensions);

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
