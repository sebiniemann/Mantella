#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2010.hpp>

// C++ Standard Library
#include <algorithm>
#include <cmath>
#include <random>

// Mantella
#include <mantella_bits/helper/rng.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob2010 {
    BlackBoxOptimisationBenchmark2010::BlackBoxOptimisationBenchmark2010(
        const unsigned int& numberOfDimensions) noexcept
      : OptimisationProblem<double>(numberOfDimensions) {
      setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - 5.0);
      setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + 5.0);
      setObjectiveValueTranslation(std::min(1000.0, std::max(-1000.0, std::cauchy_distribution<double>(0.0, 100.0)(Rng::generator))));

      // TODO Check value within the paper
      setAcceptableObjectiveValue(objectiveValueTranslation_ + 1.0e-3);

      setTranslation(arma::randu<arma::Col<double>>(numberOfDimensions_) * 8.0 - 4.0);
      setOne(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::generator) ? 1.0 : -1.0));
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
      setDeltaC101(getRandomDeltaC101());
      setLocalOptimaY101(getRandomLocalOptimaY101());
      setDeltaC21(getRandomDeltaC21());
      setLocalOptimaY21(getRandomLocalOptimaY21());
    }

    void BlackBoxOptimisationBenchmark2010::setTranslation(
        const arma::Col<double>& translation) {
      if (translation.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the translation (" + std::to_string(translation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      translation_ = translation;
    }

    void BlackBoxOptimisationBenchmark2010::setOne(
        const arma::Col<double>& one) {
      if (one.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the one vector (" + std::to_string(one.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      one_ = one;
    }

    void BlackBoxOptimisationBenchmark2010::setRotationR(
        const arma::Mat<double>& rotationR) {
      if (!rotationR.is_square()) {
        throw std::logic_error("The rotation matrix (" + std::to_string(rotationR.n_rows) + ", " + std::to_string(rotationR.n_cols) + ") must be square.");
      } else if (rotationR.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the parameter rotation maxtrix (" + std::to_string(rotationR.n_rows) + ", " + std::to_string(rotationR.n_cols) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if(arma::any(arma::vectorise(arma::abs(rotationR.i() - rotationR.t()) > 1.0e-12 * std::max(1.0, std::abs(rotationR.max()))))) {
        throw std::logic_error("The rotation matrix must be orthonormal.");
      } else if(std::abs(std::abs(arma::det(rotationR)) - 1.0) > 1.0e-12) {
        throw std::logic_error("The rotation matrix's determinant (" + std::to_string(arma::det(rotationR)) + ") must be either 1 or -1.");
      }

      rotationR_ = rotationR;
    }

    void BlackBoxOptimisationBenchmark2010::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      if (!rotationQ.is_square()) {
        throw std::logic_error("The rotation matrix (" + std::to_string(rotationQ.n_rows) + ", " + std::to_string(rotationQ.n_cols) + ") must be square.");
      } else if (rotationQ.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the parameter rotation maxtrix (" + std::to_string(rotationQ.n_rows) + ", " + std::to_string(rotationQ.n_cols) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if(arma::any(arma::vectorise(arma::abs(rotationQ.i() - rotationQ.t()) > 1.0e-12 * std::max(1.0, std::abs(rotationQ.max()))))) {
        throw std::logic_error("The rotation matrix must be orthonormal.");
      } else if(std::abs(std::abs(arma::det(rotationQ)) - 1.0) > 1.0e-12) {
        throw std::logic_error("The rotation matrix's determinant (" + std::to_string(arma::det(rotationQ)) + ") must be either 1 or -1.");
      }

      rotationQ_ = rotationQ;
    }

    void BlackBoxOptimisationBenchmark2010::setDeltaC101(
        const arma::Mat<double>& deltaC101) {
      if (deltaC101.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of each delta (" + std::to_string(deltaC101.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if (deltaC101.n_cols != 101) {
        throw std::logic_error("The number of deltas (" + std::to_string(deltaC101.n_cols) + ") must be 101.");
      }

      deltaC101_ = deltaC101;
    }

    void BlackBoxOptimisationBenchmark2010::setLocalOptimaY101(
        const arma::Mat<double>& localOptimaY101) {
    if (localOptimaY101.n_rows != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of each local optimum (" + std::to_string(localOptimaY101.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    } else if (localOptimaY101.n_cols != 101) {
      throw std::logic_error("The number of local optima (" + std::to_string(localOptimaY101.n_cols) + ") must be 101.");
    }

      localOptimaY101_ = localOptimaY101;
    }

    void BlackBoxOptimisationBenchmark2010::setDeltaC21(
        const arma::Mat<double>& deltaC21) {
      if (deltaC21.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of each delta (" + std::to_string(deltaC21.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if (deltaC21.n_cols != 21) {
        throw std::logic_error("The number of deltas (" + std::to_string(deltaC21.n_cols) + ") must be 21.");
      }

      deltaC21_ = deltaC21;
    }

    void BlackBoxOptimisationBenchmark2010::setLocalOptimaY21(
        const arma::Mat<double>& localOptimaY21) {
      if (localOptimaY21.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of each local optimum (" + std::to_string(localOptimaY21.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if (localOptimaY21.n_cols != 21) {
        throw std::logic_error("The number of local optima (" + std::to_string(localOptimaY21.n_cols) + ") must be 21.");
      }

      localOptimaY21_ = localOptimaY21;
    }

    arma::Mat<double> BlackBoxOptimisationBenchmark2010::getRandomDeltaC101() const noexcept {
      arma::Mat<double> deltaC101(numberOfDimensions_, 101);
      deltaC101.col(0) = getScaling(std::sqrt(1000.0)) / std::pow(1000.0, 0.25);

      std::uniform_int_distribution<unsigned int> uniformIntDistribution(0, 99);
      for (std::size_t n = 1; n < deltaC101.n_cols; ++n) {
        deltaC101.col(n) = getScaling(sqrt(1000.0)) / pow(pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Rng::generator)) / 99.0), 0.25);
      }

      return deltaC101;
    }

    arma::Mat<double> BlackBoxOptimisationBenchmark2010::getRandomDeltaC21() const noexcept {
      arma::Mat<double> deltaC21(numberOfDimensions_, 21);
      deltaC21.col(0) = getScaling(std::sqrt(1000.0)) / std::pow(1000.0, 0.25);

      std::uniform_int_distribution<unsigned int> uniformIntDistribution(0, 19);
      for (std::size_t n = 1; n < deltaC21.n_cols; ++n) {
        deltaC21.col(n) = getScaling(sqrt(1000.0)) / std::pow(std::pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Rng::generator)) / 19.0), 0.25);
      }

      return deltaC21;
    }

    arma::Mat<double> BlackBoxOptimisationBenchmark2010::getRandomLocalOptimaY101() const noexcept {
      arma::Mat<double> localOptimaY101 = arma::randu<arma::Mat<double>>(numberOfDimensions_, 101) * 8.0 - 4.0;
      localOptimaY101.col(0) = 0.8 * localOptimaY101.col(0);

      return localOptimaY101;
    }

    arma::Mat<double> BlackBoxOptimisationBenchmark2010::getRandomLocalOptimaY21() const noexcept {
      arma::Mat<double> localOptimaY21 = arma::randu<arma::Mat<double>>(numberOfDimensions_, 21) * 9.8 - 4.9;
      localOptimaY21.col(0) = 0.8 * localOptimaY21.col(0);

      return localOptimaY21;
    }

    arma::Col<double> BlackBoxOptimisationBenchmark2010::getScaling(
        const double& condition) const noexcept {
      arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (auto& scale : scaling) {
        scale = std::pow(condition, scale);
      }

      return scaling;
    }

    arma::Col<double> BlackBoxOptimisationBenchmark2010::getScaling(
        const arma::Col<double>& condition) const noexcept {
      arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (std::size_t n = 0; n < scaling.n_elem; ++n) {
        scaling.at(n) = std::pow(condition.at(n), scaling.at(n));
      }

      return scaling;
    }

    arma::Col<double> BlackBoxOptimisationBenchmark2010::getAsymmetricTransformation(
        const double& beta,
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> asymmetricTransformation(parameter.n_elem);
      const arma::Col<double>& spacing = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        const double& value = parameter.at(n);

        if (value > 0.0) {
          asymmetricTransformation.at(n) = std::pow(value, 1 + beta * spacing.at(n) * std::sqrt(value));
        } else {
          asymmetricTransformation.at(n) = value;
        }
      }

      return asymmetricTransformation;
    }

    double BlackBoxOptimisationBenchmark2010::getOscillationTransformation(
        const double& value) const noexcept {
      if (value != 0.0) {
        double c1;
        double c2;
        if (value > 0.0) {
          c1 = 10.0;
          c2 = 7.9;
        } else {
          c1 = 5.5;
          c2 = 3.1;
        }

        const double& x = std::log(std::abs(value));
        return std::copysign(1.0, value) * std::exp(x + 0.049 * (std::sin(c1 * x) + std::sin(c2 * x)));
      } else {
        return 0.0;
      }
    }

    arma::Col<double> BlackBoxOptimisationBenchmark2010::getOscillationTransformation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> oscillate(parameter.n_elem);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        oscillate.at(n) = getOscillationTransformation(parameter.at(n));
      }

      return oscillate;
    }

    double BlackBoxOptimisationBenchmark2010::getPenality(
        const arma::Col<double>& parameter) const noexcept {
      double penality = 0.0;

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        penality += std::pow(std::max(0.0, std::abs(parameter.at(n)) - 5.0), 2.0);
      }

      return penality;
    }
  }
}
