#include <optimisationProblem/benchmark/benchmarkProblem.hpp>

#include <algorithm>
#include <cmath>
#include <random>

#include <helper/random.hpp>

namespace hop {
  BenchmarkProblem::BenchmarkProblem(const unsigned int& numberOfDimensions) : OptimisationProblem(numberOfDimensions) {
    setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 5.0);
    setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 5.0);
    setObjectiveValueTranslation(std::min(1000.0, std::max(-1000.0, std::cauchy_distribution<double>(0.0, 100.0)(Random::RNG))));

    setTranslation(arma::randu<arma::Col<double>>(numberOfDimensions) * 8.0 - 4.0);
    setOne(arma::zeros<arma::Col<double>>(numberOfDimensions) + (std::bernoulli_distribution(0.5)(Random::RNG) ? 1.0 : -1.0));
    setRotationR(getRandomRotation());
    setRotationQ(getRandomRotation());
    setDeltaC101(getRandomDeltaC101());
    setLocalOptimaY101(getRandomLocalOptimaY101());
    setDeltaC21(getRandomDeltaC21());
    setLocalOptimaY21(getRandomLocalOptimaY21());
  }

  void BenchmarkProblem::setTranslation(const arma::Col<double>& translation) {
    _translation = translation;
  }

  void BenchmarkProblem::setOne(const arma::Col<double>& one) {
    _one = one;
  }

  void BenchmarkProblem::setRotationR(const arma::Mat<double>& rotationR) {
    _rotationR = rotationR;
  }

  void BenchmarkProblem::setRotationQ(const arma::Mat<double>& rotationQ) {
    _rotationQ = rotationQ;
  }

  void BenchmarkProblem::setDeltaC101(const arma::Mat<double>& deltaC101) {
    _deltaC101 = deltaC101;
  }

  void BenchmarkProblem::setLocalOptimaY101(const arma::Mat<double>& localOptimaY101) {
    _localOptimaY101 = localOptimaY101;
  }

  void BenchmarkProblem::setDeltaC21(const arma::Mat<double>& deltaC21) {
    _deltaC21 = deltaC21;
  }

  void BenchmarkProblem::setLocalOptimaY21(const arma::Mat<double>& localOptimaY21) {
    _localOptimaY21 = localOptimaY21;
  }

  arma::Mat<double> BenchmarkProblem::getRandomRotation() const {
    arma::Mat<double> rotationMatrix = arma::randn<arma::Mat<double>>(_numberOfDimensions, _numberOfDimensions);
    for (std::size_t j = 0; j < rotationMatrix.n_cols; j++) {
      for (unsigned int jj = 0; jj < j; jj++) {
        rotationMatrix.col(j) = rotationMatrix.col(j) - arma::dot(rotationMatrix.col(j), rotationMatrix.col(jj)) * rotationMatrix.col(jj);
      }
      rotationMatrix.col(j) = rotationMatrix.col(j) / arma::norm(rotationMatrix.col(j));
    }

    return rotationMatrix;
  }

  arma::Mat<double> BenchmarkProblem::getRandomDeltaC101() const {
    arma::Mat<double> deltaC101(_numberOfDimensions, 101);
    deltaC101.col(0) = getScaling(std::sqrt(1000.0)) / std::pow(1000.0, 0.25);

    std::uniform_int_distribution<int> uniformIntDistribution(0, 99);
    for(std::size_t j = 1; j < deltaC101.n_cols; j++) {
      deltaC101.col(j) = getScaling(sqrt(1000.0)) / pow(pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Random::RNG)) / 99.0), 0.25);
    }

    return deltaC101;
  }

  arma::Mat<double> BenchmarkProblem::getRandomDeltaC21() const {
    arma::Mat<double> deltaC21(_numberOfDimensions, 21);
    deltaC21.col(0) = getScaling(std::sqrt(1000.0)) / std::pow(1000.0, 0.25);

    std::uniform_int_distribution<int> uniformIntDistribution(0, 19);
    for(std::size_t j = 1; j < deltaC21.n_cols; j++) {
      deltaC21.col(j) = getScaling(sqrt(1000.0)) / std::pow(std::pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Random::RNG)) / 19.0), 0.25);
    }

    return deltaC21;
  }

<<<<<<< HEAD
  Mat<double> BenchmarkProblem::getRandomLocalOptimaY101() const {
    Mat<double> localOptimaY101 = randu(_numberOfDimensions, 101) * 10.0 - 5.0;
=======
  arma::Mat<double> BenchmarkProblem::getRandomLocalOptimaY101() const {
    arma::Mat<double> localOptimaY101 = arma::randu<arma::Mat<double>>(_numberOfDimensions, 101) * 8.0 - 4.0;
>>>>>>> devel: Removed "using .." notation and added namespace directly
    localOptimaY101.col(0) = 0.8 * localOptimaY101.col(0);

    return localOptimaY101;
  }

  arma::Mat<double> BenchmarkProblem::getRandomLocalOptimaY21() const {
    arma::Mat<double> localOptimaY21 = arma::randu<arma::Mat<double>>(_numberOfDimensions, 21) * 9.8 - 4.9;
    localOptimaY21.col(0) = 0.8 * localOptimaY21.col(0);

    return localOptimaY21;
  }

  arma::Col<double> BenchmarkProblem::getScaling(const double& condition) const {
    arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0, 1, _numberOfDimensions);

    for (auto& scale : scaling) {
      scale = std::pow(condition, scale);
    }

    return scaling;
  }

  arma::Col<double> BenchmarkProblem::getScaling(const arma::Col<double>& condition) const {
    arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0, 1, _numberOfDimensions);

    for (size_t n = 0; n < scaling.n_elem; n++) {
      scaling.at(n) = std::pow(condition.at(n), scaling.at(n));
    }

    return scaling;
  }

  arma::Col<double> BenchmarkProblem::getAsymmetricTransformation(const double& beta, const arma::Col<double>& parameter) const {
    arma::Col<double> asymmetricTransformation(parameter.n_elem);
    arma::Col<double> spacing = arma::linspace<arma::Col<double>>(0, 1, _numberOfDimensions);

    for (size_t n = 0; n < parameter.n_elem; n++) {
      double value = parameter.at(n);

      if (value > 0.0) {
        asymmetricTransformation.at(n) = std::pow(value, 1 + beta * spacing.at(n) * std::sqrt(value));
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

      double x = std::log(std::abs(value));
      return std::copysign(1.0, value) * std::exp(x + 0.049 * (std::sin(c1 * x) + std::sin(c2 * x)));
    } else {
      return 0.0;
    }
  }

  arma::Col<double> BenchmarkProblem::getOscillationTransformation(const arma::Col<double>& parameter) const {
    arma::Col<double> oscillate(parameter.n_elem);

    for (std::size_t n = 0; n < parameter.n_elem; n++) {
      oscillate.at(n) = getOscillationTransformation(parameter.at(n));
    }

    return oscillate;
  }

  double BenchmarkProblem::getPenality(const arma::Col<double>& parameter) const {
    double penality = 0.0;

    for (std::size_t n = 0; n < parameter.n_elem; n++) {
      penality += std::pow(std::max(0.0, std::abs(parameter.at(n)) - 5.0), 2.0);
    }

    return penality;
  }
}
