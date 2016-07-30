#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian101mePeaksFunction.hpp"

// C++ standard library
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/mpi.hpp"
#include "mantella_bits/probability.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          weights_(arma::join_cols(arma::vec({10.0}), arma::linspace<arma::vec>(1.1, 9.1, 100))),
          rotationQ_(synchronise(randomRotationMatrix(numberOfDimensions_))),
          localParameterTranslations_(synchronise(arma::mat(uniformRandomNumbers(numberOfDimensions_, 101) * 10.0 - 5.0))) {
      assert(numberOfDimensions_ > 1 && "GallaghersGaussian101mePeaksFunction: The number of dimensions must be greater than 1.");

      if (!isRepresentableAsFloatingPoint(numberOfDimensions_)) {
        throw std::range_error("GallaghersGaussian101mePeaksFunction: The number of elements must be representable as a floating point.");
      }

      localParameterTranslations_.col(0) = 0.8 * localParameterTranslations_.col(0);

      // Calculates the local parameter conditionings.
      arma::vec::fixed<101> conditionNumbers;
      conditionNumbers(0) = 49.5;
      conditionNumbers.tail(conditionNumbers.n_elem - 1) = arma::conv_to<arma::vec>::from(randomPermutationVector(conditionNumbers.n_elem - 1, conditionNumbers.n_elem - 1));
      // Only the condition numbers are randomly drawn and therefore need to be
      // synchronise.
      conditionNumbers = synchronise(conditionNumbers);

      localParameterConditionings_.set_size(numberOfDimensions_, conditionNumbers.n_elem);
      for (arma::uword n = 0; n < conditionNumbers.n_elem; ++n) {
        const double condition = std::pow(10.0, conditionNumbers(n) / 33.0);
        localParameterConditionings_.col(n) = getParameterConditioning(condition) / std::sqrt(condition);
      }

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              double maximalValue = std::numeric_limits<decltype(maximalValue)>::lowest();
              for (arma::uword n = 0; n < 101; ++n) {
                const arma::vec& localParameterTranslation = parameter_ - localParameterTranslations_.col(n);
                maximalValue = std::max(maximalValue, weights_(n) * std::exp(-0.5 / static_cast<decltype(maximalValue)>(numberOfDimensions_) * arma::dot(localParameterTranslation, rotationQ_.t() * arma::diagmat(localParameterConditionings_.col(n)) * rotationQ_ * localParameterTranslation)));
              }

              return std::pow(getOscillatedValue(10.0 - maximalValue), 2.0);
            },
            "BBOB Gallagher's Gaussian 101-me Peaks Function (f21)"}});
    }
  }
}
