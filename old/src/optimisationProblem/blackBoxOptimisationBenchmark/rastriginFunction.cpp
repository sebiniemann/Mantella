#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/rastriginFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

// Mantella
#include "mantella_bits/assert.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    RastriginFunction::RastriginFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      assert(numberOfDimensions_ > 1 && "RastriginFunction: The number of dimensions must be greater than 1.");

      if (!isRepresentableAsFloatingPoint(numberOfDimensions_)) {
        throw std::range_error("RastriginFunction: The number of elements must be representable as a floating point.");
      }

      setParameterTranslation(getRandomParameterTranslation());

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              const arma::vec& z = parameterConditioning_ % getAsymmetricParameter(0.2, getOscillatedParameter(parameter_));
              return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0);
            },
            "BBOB Rastrigin Function (f3)"}});
    }
  }
}
