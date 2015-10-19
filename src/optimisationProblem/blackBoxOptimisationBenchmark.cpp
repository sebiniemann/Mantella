#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

// C++ standard library
#include <random>
#include <cmath>
#include <algorithm>

// Mantella
#include "mantella_bits/helper/rng.hpp"

namespace mant {
  namespace bbob {
    BlackBoxOptimisationBenchmark::BlackBoxOptimisationBenchmark(
        const arma::uword numberOfDimensions)
        : OptimisationProblem(numberOfDimensions) {
      // A vector with all elements set to -5.
      setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - 5.0);
      // A vector with all elements set to 5.
      setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + 5.0);
      // The objective value translation is randomly chosen from a Cauchy distribution with an approximate 50% chance to be within [-100, 100], rounded up to 2 decimal places.
      // The translation is further bounded between -1000 and 1000.
      setObjectiveValueTranslation(std::min(1000.0, std::max(-1000.0, std::floor(std::cauchy_distribution<double>(0.0, 100.0)(Rng::getGenerator()) * 100.0) / 100.0)));
      setAcceptableObjectiveValue(objectiveValueTranslation_ + 1.0e-8);
    }

    arma::Col<double> BlackBoxOptimisationBenchmark::getRandomParameterTranslation() const {
      // The parameter space is randomly translated by [-4, 4]^N, rounded up to 4 decimal places.
      arma::Col<double> randomParameterTranslation = arma::floor(arma::randu<arma::Col<double>>(numberOfDimensions_) * 1.0e4) / 1.0e4 * 8.0 - 4.0;
      // In case the parameter space would remain unchanged, it is forcefully translated by -0.00001.
      randomParameterTranslation.elem(arma::find(randomParameterTranslation == 0.0)).fill(-1.0e-5);

      return randomParameterTranslation;
    }

    arma::Col<double> BlackBoxOptimisationBenchmark::getParameterConditioning(
        const double conditionNumber) const {
      arma::Col<double> parameterConditioning = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      // In-place calculation of the conditioning.
      for (auto& conditioning : parameterConditioning) {
        conditioning = std::pow(conditionNumber, conditioning);
      }

      return parameterConditioning;
    }

    arma::Col<double> BlackBoxOptimisationBenchmark::getConditionedParameter(
        const arma::Col<double>& parameter) const {
      arma::Col<double> conditionedParameter = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (arma::uword n = 0; n < parameter.n_elem; ++n) {
        conditionedParameter(n) = std::pow(parameter(n), conditionedParameter(n));
      }

      return conditionedParameter;
    }

    arma::Col<double> BlackBoxOptimisationBenchmark::getAsymmetricParameter(
        const double asymmetry,
        const arma::Col<double>& parameter) const {
      arma::Col<double> asymmetricParameter(parameter.n_elem);
      const arma::Col<double>& spacing = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (arma::uword n = 0; n < parameter.n_elem; ++n) {
        const double value = parameter(n);

        if (value > 0.0) {
          asymmetricParameter(n) = std::pow(value, 1.0 + asymmetry * spacing(n) * std::sqrt(value));
        } else {
          asymmetricParameter(n) = value;
        }
      }

      return asymmetricParameter;
    }

    double BlackBoxOptimisationBenchmark::getOscillatedValue(
        const double value) const {
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

        const double logAbsoluteValue = std::log(std::abs(value));
        return std::copysign(1.0, value) * std::exp(logAbsoluteValue + 0.049 * (std::sin(c1 * logAbsoluteValue) + std::sin(c2 * logAbsoluteValue)));
      } else {
        return 0.0;
      }
    }

    arma::Col<double> BlackBoxOptimisationBenchmark::getOscillatedParameter(
        const arma::Col<double>& parameter) const {
      arma::Col<double> oscillatedParameter(parameter.n_elem);

      for (arma::uword n = 0; n < parameter.n_elem; ++n) {
        oscillatedParameter(n) = getOscillatedValue(parameter(n));
      }

      return oscillatedParameter;
    }

    double BlackBoxOptimisationBenchmark::getBoundConstraintsValue(
        const arma::Col<double>& parameter) const {
      double boundConstraintsValue = 0.0;

      for (arma::uword n = 0; n < parameter.n_elem; ++n) {
        boundConstraintsValue += std::pow(std::max(0.0, std::abs(parameter(n)) - 5.0), 2.0);
      }

      return boundConstraintsValue;
    }

#if defined(SUPPORT_MPI)
    std::vector<double> BlackBoxOptimisationBenchmark::serialise() const {
      return OptimisationProblem::serialise();
    }

    void BlackBoxOptimisationBenchmark::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      OptimisationProblem::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
