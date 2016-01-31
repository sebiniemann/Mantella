#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/rosenbrockFunctionRotated.hpp"

// C++ standard library
#include <cassert>
#include <cmath>
#include <algorithm>

// Mantella
#include "mantella_bits/probability.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    RosenbrockFunctionRotated::RosenbrockFunctionRotated(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          max_(std::max(1.0, std::sqrt(numberOfDimensions_) / 8.0)) {
      setParameterRotation(randomRotationMatrix(numberOfDimensions_));

      setObjectiveFunction(
          [this](
              const arma::Col<double>& parameter_) {
            assert(parameter_.n_elem == numberOfDimensions_);
              
            const arma::Col<double>& z = max_ * parameter_ + 0.5;
            return 100.0 * std::pow(arma::norm(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1)), 2.0) + std::pow(arma::norm(z.head(z.n_elem - 1) - 1.0), 2.0);
          },
          "BBOB Rosenbrock Function, rotated (f9)");
    }
  }
}
