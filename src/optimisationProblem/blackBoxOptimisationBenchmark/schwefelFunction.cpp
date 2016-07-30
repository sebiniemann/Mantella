#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/schwefelFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>
#include <functional>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>

// Mantella
#include "mantella_bits/assert.hpp"
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
      assert(numberOfDimensions_ > 1 && "SchwefelFunction: The number of dimensions must be greater than 1.");

      if (!isRepresentableAsFloatingPoint(numberOfDimensions_)) {
        throw std::range_error("SchwefelFunction: The number of elements must be representable as a floating point.");
      }

      // A vector with all elements randomly and uniformly set to either 2 or -2.
      setParameterScaling(arma::zeros<arma::vec>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 2.0 : -2.0));

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              arma::vec s = parameter_;
              s.tail(s.n_elem - 1) += 0.25 * (s.head(s.n_elem - 1) - 4.2096874633);

              const arma::vec& z = 100.0 * (parameterConditioning_ % (s - 4.2096874633) + 4.2096874633);

              return 0.01 * (418.9828872724339 - arma::dot(z, arma::sin(arma::sqrt(arma::abs(z)))) / static_cast<double>(numberOfDimensions_)) + 100.0 * arma::accu(arma::pow(arma::max(arma::zeros<arma::vec>(numberOfDimensions_), arma::abs(z / 100.0) - 5.0), 2.0));
            },
            "BBOB Schwefel Function (f20)"}});
    }
  }
}
