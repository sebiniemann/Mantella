#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/attractiveSectorFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>
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
    AttractiveSectorFunction::AttractiveSectorFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))),
          rotationQ_(synchronise(randomRotationMatrix(numberOfDimensions_))) {
      assert(numberOfDimensions_ > 1 && "AttractiveSectorFunction: The number of dimensions must be greater than 1.");

      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(randomRotationMatrix(numberOfDimensions_));

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              arma::vec z = rotationQ_ * (parameterConditioning_ % parameter_);
              z.elem(arma::find(z % parameterTranslation_ > 0.0)) *= 100.0;

              return std::pow(getOscillatedValue(std::pow(arma::norm(z), 2.0)), 0.9);
            },
            "BBOB Attractive Sector Function (f6)"}});
    }
  }
}
