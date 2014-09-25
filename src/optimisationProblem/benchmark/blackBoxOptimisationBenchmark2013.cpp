#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

#include <algorithm>
#include <cmath>
#include <random>

#include <hop_bits/helper/random.hpp>

namespace hop {
  BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013(const unsigned int& numberOfDimensions)
    : OptimisationProblem(numberOfDimensions) {
    setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - 5.0);
    setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + 5.0);
    setObjectiveValueTranslation(std::min(1000.0, std::max(-1000.0, std::cauchy_distribution<double>(0.0, 100.0)(Random::Rng))));

    setAcceptableObjectiveValue(objectiveValueTranslation_ + 10e-3);

    setTranslation(arma::randu<arma::Col<double>>(numberOfDimensions_) * 8.0 - 4.0);
    setOne(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Random::Rng) ? 1.0 : -1.0));
    setRotationR(getRandomRotation());
    setRotationQ(getRandomRotation());
    setDeltaC101(getRandomDeltaC101());
    setLocalOptimaY101(getRandomLocalOptimaY101());
    setDeltaC21(getRandomDeltaC21());
    setLocalOptimaY21(getRandomLocalOptimaY21());
  }

  void BlackBoxOptimisationBenchmark2013::setTranslation(const arma::Col<double>& translation) {
    translation_ = translation;
  }

  void BlackBoxOptimisationBenchmark2013::setOne(const arma::Col<double>& one) {
    one_ = one;
  }

  void BlackBoxOptimisationBenchmark2013::setRotationR(const arma::Mat<double>& rotationR) {
    rotationR_ = rotationR;
  }

  void BlackBoxOptimisationBenchmark2013::setRotationQ(const arma::Mat<double>& rotationQ) {
    rotationQ_ = rotationQ;
  }

  void BlackBoxOptimisationBenchmark2013::setDeltaC101(const arma::Mat<double>& deltaC101) {
    deltaC101_ = deltaC101;
  }

  void BlackBoxOptimisationBenchmark2013::setLocalOptimaY101(const arma::Mat<double>& localOptimaY101) {
    localOptimaY101_ = localOptimaY101;
  }

  void BlackBoxOptimisationBenchmark2013::setDeltaC21(const arma::Mat<double>& deltaC21) {
    deltaC21_ = deltaC21;
  }

  void BlackBoxOptimisationBenchmark2013::setLocalOptimaY21(const arma::Mat<double>& localOptimaY21) {
    localOptimaY21_ = localOptimaY21;
  }

  arma::Mat<double> BlackBoxOptimisationBenchmark2013::getRandomRotation() const {
    arma::Mat<double> rotationMatrix = arma::randn<arma::Mat<double>>(numberOfDimensions_, numberOfDimensions_);
    for (std::size_t j = 0; j < rotationMatrix.n_cols; ++j) {
      for (unsigned int jj = 0; jj < j; ++jj) {
        rotationMatrix.col(j) = rotationMatrix.col(j) - arma::dot(rotationMatrix.col(j), rotationMatrix.col(jj)) * rotationMatrix.col(jj);
      }
      rotationMatrix.col(j) = rotationMatrix.col(j) / arma::norm(rotationMatrix.col(j));
    }

    return rotationMatrix;
  }

  arma::Mat<double> BlackBoxOptimisationBenchmark2013::getRandomDeltaC101() const {
    arma::Mat<double> deltaC101(numberOfDimensions_, 101);
    deltaC101.col(0) = getScaling(std::sqrt(1000.0)) / std::pow(1000.0, 0.25);

    std::uniform_int_distribution<int> uniformIntDistribution(0, 99);
    for(std::size_t j = 1; j < deltaC101.n_cols; ++j) {
      deltaC101.col(j) = getScaling(sqrt(1000.0)) / pow(pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Random::Rng)) / 99.0), 0.25);
    }

    return deltaC101;
  }

  arma::Mat<double> BlackBoxOptimisationBenchmark2013::getRandomDeltaC21() const {
    arma::Mat<double> deltaC21(numberOfDimensions_, 21);
    deltaC21.col(0) = getScaling(std::sqrt(1000.0)) / std::pow(1000.0, 0.25);

    std::uniform_int_distribution<int> uniformIntDistribution(0, 19);
    for(std::size_t j = 1; j < deltaC21.n_cols; ++j) {
      deltaC21.col(j) = getScaling(sqrt(1000.0)) / std::pow(std::pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Random::Rng)) / 19.0), 0.25);
    }

    return deltaC21;
  }

  arma::Mat<double> BlackBoxOptimisationBenchmark2013::getRandomLocalOptimaY101() const {
    arma::Mat<double> localOptimaY101 = arma::randu<arma::Mat<double>>(numberOfDimensions_, 101) * 8.0 - 4.0;
    localOptimaY101.col(0) = 0.8 * localOptimaY101.col(0);

    return localOptimaY101;
  }

  arma::Mat<double> BlackBoxOptimisationBenchmark2013::getRandomLocalOptimaY21() const {
    arma::Mat<double> localOptimaY21 = arma::randu<arma::Mat<double>>(numberOfDimensions_, 21) * 9.8 - 4.9;
    localOptimaY21.col(0) = 0.8 * localOptimaY21.col(0);

    return localOptimaY21;
  }

  arma::Col<double> BlackBoxOptimisationBenchmark2013::getScaling(const double& condition) const {
    arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0, 1, numberOfDimensions_);

    for (auto& scale : scaling) {
      scale = std::pow(condition, scale);
    }

    return scaling;
  }

  arma::Col<double> BlackBoxOptimisationBenchmark2013::getScaling(const arma::Col<double>& condition) const {
    arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0, 1, numberOfDimensions_);

    for (size_t n = 0; n < scaling.n_elem; ++n) {
      scaling.at(n) = std::pow(condition.at(n), scaling.at(n));
    }

    return scaling;
  }

  arma::Col<double> BlackBoxOptimisationBenchmark2013::getAsymmetricTransformation(const double& beta, const arma::Col<double>& parameter) const {
    arma::Col<double> asymmetricTransformation(parameter.n_elem);
    arma::Col<double> spacing = arma::linspace<arma::Col<double>>(0, 1, numberOfDimensions_);

    for (size_t n = 0; n < parameter.n_elem; ++n) {
      double value = parameter.at(n);

      if (value > 0.0) {
        asymmetricTransformation.at(n) = std::pow(value, 1 + beta * spacing.at(n) * std::sqrt(value));
      } else {
        asymmetricTransformation.at(n) = value;
      }
    }

    return asymmetricTransformation;
  }

  double BlackBoxOptimisationBenchmark2013::getOscillationTransformation(const double& value) const {
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

  arma::Col<double> BlackBoxOptimisationBenchmark2013::getOscillationTransformation(const arma::Col<double>& parameter) const {
    arma::Col<double> oscillate(parameter.n_elem);

    for (std::size_t n = 0; n < parameter.n_elem; ++n) {
      oscillate.at(n) = getOscillationTransformation(parameter.at(n));
    }

    return oscillate;
  }

  double BlackBoxOptimisationBenchmark2013::getPenality(const arma::Col<double>& parameter) const {
    double penality = 0.0;

    for (std::size_t n = 0; n < parameter.n_elem; ++n) {
      penality += std::pow(std::max(0.0, std::abs(parameter.at(n)) - 5.0), 2.0);
    }

    return penality;
  }
}
