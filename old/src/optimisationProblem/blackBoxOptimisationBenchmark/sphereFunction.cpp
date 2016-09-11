#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/sphereFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>
#include <functional>
#include <string>
#include <utility>

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    SphereFunction::SphereFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions) {
      assert(numberOfDimensions_ > 1 && "SphereFunction: The number of dimensions must be greater than 1.");

      setParameterTranslation(getRandomParameterTranslation());

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              return std::pow(arma::norm(parameter_), 2.0);
            },
            "BBOB Sphere Function (f1)"}});
    }
  }
}
