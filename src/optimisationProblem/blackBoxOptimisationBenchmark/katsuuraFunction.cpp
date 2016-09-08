#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/katsuuraFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>
#include <functional>
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
    KatsuuraFunction::KatsuuraFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(10.0)),
          rotationQ_(synchronise(randomRotationMatrix(numberOfDimensions_))) {
      assert(numberOfDimensions_ > 1 && "KatsuuraFunction: The number of dimensions must be greater than 1.");

      if (!isRepresentableAsFloatingPoint(numberOfDimensions_)) {
        throw std::range_error("KatsuuraFunction: The number of elements must be representable as a floating point.");
      }

      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(randomRotationMatrix(numberOfDimensions_));

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              arma::vec z = rotationQ_ * (parameterConditioning_ % parameter_);

              double product = 1.0;
              for (arma::uword n = 0; n < z.n_elem; ++n) {
                const double value = z(n);

                double sum = 0.0;
                for (arma::uword k = 1; k < 33; ++k) {
                  const double power = std::pow(2.0, k);
                  sum += std::abs(power * value - std::round(power * value)) / power;
                }

                product *= std::pow(1.0 + (static_cast<decltype(product)>(n) + 1.0) * sum, 10.0 / std::pow(numberOfDimensions_, 1.2));
              }

              return 10.0 / std::pow(numberOfDimensions_, 2.0) * (product - 1.0);
            },
            "BBOB Katsuura Function (f23)"}});
    }
  }
}
