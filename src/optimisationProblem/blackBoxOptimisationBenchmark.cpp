#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark.hpp"

// C++ standard library
#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <stdexcept>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/probability.hpp"
#include "mantella_bits/randomNumberGenerator.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    BlackBoxOptimisationBenchmark::BlackBoxOptimisationBenchmark(
        const arma::uword numberOfDimensions)
        : OptimisationProblem(numberOfDimensions) {
      setLowerBounds(arma::zeros<arma::vec>(numberOfDimensions_) - 5.0);
      setUpperBounds(arma::zeros<arma::vec>(numberOfDimensions_) + 5.0);

      setObjectiveValueTranslation(getRandomObjectiveValueTranslation());
    }

    arma::vec BlackBoxOptimisationBenchmark::getRandomParameterTranslation() {
      // The parameter space is randomly translated by [-4, 4]^N, rounded up to 4 decimal places.
      arma::vec randomParameterTranslation = arma::floor(uniformRandomNumbers(numberOfDimensions_, std::uniform_real_distribution<double>(-4.0, 4.0)) * 1e4) / 1e4;
      // In case the parameter space would remain unchanged, it is forcefully translated by -0.00001.
      randomParameterTranslation.elem(arma::find(randomParameterTranslation == 0.0)).fill(-1e-5);

      return randomParameterTranslation;
    }

    arma::vec BlackBoxOptimisationBenchmark::getParameterConditioning(
        const double conditionNumber) {
      if (!isRepresentableAsFloatingPoint(numberOfDimensions_)) {
        throw std::range_error("BlackBoxOptimisationBenchmark.parameterConditioning: The number of elements must be representable as a floating point.");
      }

      arma::vec parameterConditioning(numberOfDimensions_);
      for (arma::uword n = 0; n < numberOfDimensions_; ++n) {
        parameterConditioning(n) = std::pow(conditionNumber, static_cast<decltype(conditionNumber)>(n) / (static_cast<decltype(conditionNumber)>(numberOfDimensions_) - 1.0));
      }

      return parameterConditioning;
    }

    arma::vec BlackBoxOptimisationBenchmark::getConditionedParameter(
        const arma::vec& parameter) {
      assert(parameter.n_elem == numberOfDimensions_ && "BlackBoxOptimisationBenchmark.getConditionedParameter: The parameter's number of elements must be equal to the optimisation problem's number of dimensions.");

      if (!isRepresentableAsFloatingPoint(numberOfDimensions_)) {
        throw std::range_error("BlackBoxOptimisationBenchmark.getConditionedParameter: The number of elements must be representable as a floating point.");
      }

      arma::vec conditionedParameter(numberOfDimensions_);
      for (arma::uword n = 0; n < numberOfDimensions_; ++n) {
        conditionedParameter(n) = std::pow(parameter(n), static_cast<decltype(parameter(0))>(n) / (static_cast<decltype(parameter(0))>(numberOfDimensions_) - 1.0));
      }

      return conditionedParameter;
    }

    arma::vec BlackBoxOptimisationBenchmark::getAsymmetricParameter(
        const double asymmetry,
        const arma::vec& parameter) {
      assert(parameter.n_elem == numberOfDimensions_ && "BlackBoxOptimisationBenchmark.asymmetricParameter: The parameter's number of elements must be equal to the optimisation problem's number of dimensions.");

      if (!isRepresentableAsFloatingPoint(numberOfDimensions_)) {
        throw std::range_error("BlackBoxOptimisationBenchmark.asymmetricParameter: The number of elements must be representable as a floating point.");
      }

      arma::vec asymmetricParameter(numberOfDimensions_);
      for (arma::uword n = 0; n < numberOfDimensions_; ++n) {
        const double value = parameter(n);

        if (value > 0.0) {
          asymmetricParameter(n) = std::pow(value, 1.0 + asymmetry * std::sqrt(value) * static_cast<decltype(value)>(n) / (static_cast<decltype(value)>(numberOfDimensions_) - 1.0));
        } else {
          asymmetricParameter(n) = value;
        }
      }

      return asymmetricParameter;
    }

    arma::vec BlackBoxOptimisationBenchmark::getOscillatedParameter(
        const arma::vec& parameter) {
      assert(parameter.n_elem == numberOfDimensions_ && "BlackBoxOptimisationBenchmark.oscillatedParameter: The parameter's number of elements must be equal to the optimisation problem's number of dimensions.");

      arma::vec oscillatedParameter(numberOfDimensions_);
      for (arma::uword n = 0; n < numberOfDimensions_; ++n) {
        oscillatedParameter(n) = getOscillatedValue(parameter(n));
      }

      return oscillatedParameter;
    }

    double BlackBoxOptimisationBenchmark::getRandomObjectiveValueTranslation() {
      return std::min(1000.0, std::max(-1000.0, std::floor(std::cauchy_distribution<double>(0.0, 100.0)(Rng::getGenerator()) * 1e2) / 1e2));
    }

    double BlackBoxOptimisationBenchmark::getOscillatedValue(
        const double value) {
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
  }
}
