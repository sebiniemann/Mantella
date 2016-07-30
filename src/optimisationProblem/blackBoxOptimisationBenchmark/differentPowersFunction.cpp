#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/differentPowersFunction.hpp"

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
    DifferentPowersFunction::DifferentPowersFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions) {
      assert(numberOfDimensions_ > 1 && "DifferentPowersFunction: The number of dimensions must be greater than 1.");

      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(randomRotationMatrix(numberOfDimensions_));

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              const arma::vec& z = arma::abs(parameter_);
              return arma::norm(z % getConditionedParameter(arma::square(z)));
            },
            "BBOB Different Powers Function (f14)"}});
    }
  }
}
