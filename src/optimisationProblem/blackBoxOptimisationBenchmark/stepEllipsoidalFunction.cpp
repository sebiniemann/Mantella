#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/stepEllipsoidalFunction.hpp"

// C++ standard library
#include <algorithm>
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
    StepEllipsoidalFunction::StepEllipsoidalFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          firstParameterConditioning_(getParameterConditioning(std::sqrt(10.0))),
          secondParameterConditioning_(getParameterConditioning(100.0)),
          rotationQ_(synchronise(randomRotationMatrix(numberOfDimensions_))) {
      assert(numberOfDimensions_ > 1 && "StepEllipsoidalFunction: The number of dimensions must be greater than 1.");

      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(randomRotationMatrix(numberOfDimensions_));

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              const arma::vec& s = firstParameterConditioning_ % parameter_;

              arma::vec z = s;
              for (arma::uword n = 0; n < z.n_elem; ++n) {
                const double value = s(n);

                if (std::abs(value) > 0.5) {
                  z(n) = std::round(value);
                } else {
                  z(n) = std::round(value * 10.0) / 10.0;
                }
              }

              return 0.1 * std::max(std::abs(s(0)) / 10000.0, arma::dot(secondParameterConditioning_, arma::square(rotationQ_ * z)));
            },
            "BBOB Step Ellipsoidal Function (f7)"}});
    }
  }
}
