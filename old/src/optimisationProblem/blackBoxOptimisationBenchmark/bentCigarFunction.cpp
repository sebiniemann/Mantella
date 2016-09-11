#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/bentCigarFunction.hpp"

// C++ standard library
#include <cassert>
#include <functional>
#include <string>
#include <utility>

// Mantella
#include "mantella_bits/mpi.hpp"
#include "mantella_bits/probability.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    BentCigarFunction::BentCigarFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          rotationQ_(synchronise(randomRotationMatrix(numberOfDimensions_))) {
      assert(numberOfDimensions_ > 1 && "BentCigarFunction: The number of dimensions must be greater than 1.");

      setParameterTranslation(getRandomParameterTranslation());

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              const arma::vec& z = arma::square(rotationQ_ * getAsymmetricParameter(0.5, rotationQ_ * parameter_));
              return z(0) + 1000000.0 * arma::accu(z.tail(z.n_elem - 1));
            },
            "BBOB Bent Cigar Function (f12)"}});
    }
  }
}
