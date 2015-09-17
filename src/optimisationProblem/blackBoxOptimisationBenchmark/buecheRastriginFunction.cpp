#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/buecheRastriginFunction.hpp>

// C++ standard library
#include <cassert>
#include <cmath>

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
    }

    double BuecheRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      arma::Col<double> z = parameterConditioning_ % getOscillatedParameter(parameter);
      for (arma::uword n = 0; n < z.n_elem; n += 2) {
        if (z(n) > 0.0) {
          z(n) *= 10.0;
        }
      }

      return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0);
    }

    std::string BuecheRastriginFunction::toString() const {
      return "bbob_bueche_rastrigin_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> BuecheRastriginFunction::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void BuecheRastriginFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
