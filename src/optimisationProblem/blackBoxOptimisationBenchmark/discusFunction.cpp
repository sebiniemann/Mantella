#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/discusFunction.hpp"

// C++ standard library
#include <cassert>
#include <functional>
#include <string>
#include <utility>

// Mantella
#include "mantella_bits/probability.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    DiscusFunction::DiscusFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions) {
      assert(numberOfDimensions_ > 1 && "DiscusFunction: The number of dimensions must be greater than 1.");

      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(randomRotationMatrix(numberOfDimensions_));

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              const arma::vec& z = arma::square(getOscillatedParameter(parameter_));
              return 1000000.0 * z(0) + arma::accu(z.tail(z.n_elem - 1));
            },
            "BBOB Discus Function (f11)"}});
    }
  }
}
