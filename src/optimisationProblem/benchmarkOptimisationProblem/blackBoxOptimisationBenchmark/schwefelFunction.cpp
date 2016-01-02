#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/schwefelFunction.hpp"

// C++ standard library
#include <cassert>
#include <random>
#include <cmath>

// Mantella
#include "mantella_bits/randomNumberGenerator.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    SchwefelFunction::SchwefelFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      setParameterScaling(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 2.0 : -2.0));

      setObjectiveFunction(
          [this](
              const arma::Col<double>& parameter) {
            assert(parameter.n_elem == numberOfDimensions_);
              
            arma::Col<double> s = parameter;
            s.tail(s.n_elem - 1) += 0.25 * (s.head(s.n_elem - 1) - 4.2096874633);

            const arma::Col<double>& z = 100.0 * (parameterConditioning_ % (s - 4.2096874633) + 4.2096874633);

            return 0.01 * (418.9828872724339 - arma::dot(z, arma::sin(arma::sqrt(arma::abs(z)))) / static_cast<double>(numberOfDimensions_)) + 100.0 * getBoundaryPenalty(z / 100.0);
          },
          "BBOB Schwefel Function");
    }
  }
}
