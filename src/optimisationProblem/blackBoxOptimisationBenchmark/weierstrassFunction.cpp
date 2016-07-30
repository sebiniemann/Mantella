#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/weierstrassFunction.hpp"

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
    WeierstrassFunction::WeierstrassFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(0.01))),
          rotationR_(synchronise(randomRotationMatrix(numberOfDimensions_))),
          rotationQ_(synchronise(randomRotationMatrix(numberOfDimensions_))) {
      assert(numberOfDimensions_ > 1 && "WeierstrassFunction: The number of dimensions must be greater than 1.");

      if (!isRepresentableAsFloatingPoint(numberOfDimensions_)) {
        throw std::range_error("WeierstrassFunction: The number of elements must be representable as a floating point.");
      }

      setParameterTranslation(getRandomParameterTranslation());

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              const arma::vec& z = rotationR_ * (parameterConditioning_ % (rotationQ_ * getOscillatedParameter(rotationR_ * parameter_)));

              double sum = 0.0;
              for (arma::uword n = 0; n < z.n_elem; ++n) {
                for (arma::uword k = 0; k < 12; ++k) {
                  sum += std::pow(0.5, k) * std::cos(2.0 * arma::datum::pi * std::pow(3.0, k) * (z(n) + 0.5));
                }
              }

              return 10.0 * std::pow(sum / static_cast<decltype(sum)>(numberOfDimensions_) + 1.99951171875, 3.0);
            },
            "BBOB Weierstrass Function (f16)"}});
    }
  }
}
