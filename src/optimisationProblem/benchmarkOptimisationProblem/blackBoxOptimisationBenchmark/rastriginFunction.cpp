#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/rastriginFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    RastriginFunction::RastriginFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      setParameterTranslation(getRandomParameterTranslation());

      setObjectiveFunction(
          [this](
              const arma::Col<double>& parameter) {
            assert(parameter.n_elem == numberOfDimensions_);
              
            const arma::Col<double>& z = parameterConditioning_ % getAsymmetricParameter(0.2, getOscillatedParameter(parameter));
            return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0);
          },
          "BBOB Rastrigin Function");
    }
  }
}
