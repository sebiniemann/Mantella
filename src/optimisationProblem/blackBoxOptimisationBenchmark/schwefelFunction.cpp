#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/schwefelFunction.hpp"

// C++ standard library
#include <cassert>
#include <random>
#include <cmath>

// Mantella
#include "mantella_bits/helper/rng.hpp"

namespace mant {
  namespace bbob {
    SchwefelFunction::SchwefelFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      setParameterScaling(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 2.0 : -2.0));
    }

    double SchwefelFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      arma::Col<double> s = parameter;
      s.tail(s.n_elem - 1) += 0.25 * (s.head(s.n_elem - 1) - 4.2096874633);

      const arma::Col<double>& z = 100.0 * (parameterConditioning_ % (s - 4.2096874633) + 4.2096874633);

      return 0.01 * (418.9828872724339 - arma::dot(z, arma::sin(arma::sqrt(arma::abs(z)))) / static_cast<double>(numberOfDimensions_)) + 100.0 * getBoundConstraintsValue(z / 100.0);
    }

    std::string SchwefelFunction::toString() const {
      return "bbob_schwefel_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> SchwefelFunction::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void SchwefelFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
