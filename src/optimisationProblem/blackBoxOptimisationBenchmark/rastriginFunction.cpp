#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/rastriginFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

namespace mant {
  namespace bbob {
    RastriginFunction::RastriginFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      setParameterTranslation(getRandomParameterTranslation());
    }

    double RastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      const arma::Col<double>& z = parameterConditioning_ % getAsymmetricParameter(0.2, getOscillatedParameter(parameter));

      return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0);
    }

    std::string RastriginFunction::toString() const {
      return "bbob_rastrigin_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> RastriginFunction::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void RastriginFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
