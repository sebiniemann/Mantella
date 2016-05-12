#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/buecheRastriginFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    BuecheRastriginFunction::BuecheRastriginFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      arma::Col<double> parameterTranslation = getRandomParameterTranslation();
      for (arma::uword n = 0; n < parameterTranslation.n_elem; n += 2) {
        parameterTranslation(n) = std::abs(parameterTranslation(n));
      }
      setParameterTranslation(parameterTranslation);

      setObjectiveFunction(
          [this](
              const arma::Col<double>& parameter_) {
            assert(parameter_.n_elem == numberOfDimensions_);

            arma::Col<double> z = parameterConditioning_ % getOscillatedParameter(parameter_);
            for (arma::uword n = 0; n < z.n_elem; n += 2) {
              if (z(n) > 0.0) {
                z(n) *= 10.0;
              }
            }

            return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0);
          },
          "BBOB Büche-Rastrigin Function (f4)");
    }
  }
}
