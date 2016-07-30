#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/compositeGriewankRosenbrockFunctionF8F2.hpp"

// C++ standard library
#include <algorithm>
#include <cassert>
#include <cmath>
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
    CompositeGriewankRosenbrockFunctionF8F2::
        CompositeGriewankRosenbrockFunctionF8F2(
            const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          max_(std::max(1.0, std::sqrt(numberOfDimensions_) / 8.0)) {
      assert(numberOfDimensions_ > 1 && "CompositeGriewankRosenbrockFunctionF8F2: The number of dimensions must be greater than 1.");

      setParameterRotation(randomRotationMatrix(numberOfDimensions_));

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              const arma::vec& s = max_ * parameter_ + 0.5;
              const arma::vec& z = 100.0 * arma::square(arma::square(s.head(s.n_elem - 1)) - s.tail(s.n_elem - 1)) + arma::square(s.head(s.n_elem - 1) - 1.0);

              return 10.0 * (arma::mean(z / 4000.0 - arma::cos(z)) + 1.0);
            },
            "BBOB Composite Griewank Rosenbrock Function F8F2 (f19)"}});
    }
  }
}
