#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/linearSlope.hpp"

// C++ standard library
#include <cassert>
#include <random>

// Mantella
#include "mantella_bits/randomNumberGenerator.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    LinearSlope::LinearSlope(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(10.0)),
          f0_(5.0 * arma::accu(parameterConditioning_)) {
      setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions_, numberOfDimensions_) * (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 1.0 : -1.0));

      setObjectiveFunction([this](
                               const arma::Col<double>& parameter) {
          assert(parameter.n_elem == numberOfDimensions_);
            
          arma::Col<double> z = parameter;
          z.elem(arma::find(parameter >= 5.0)).fill(5.0);

          return f0_ - arma::dot(parameterConditioning_, z);
      },
          "BBOB Linear Slope");
    }
  }
}
