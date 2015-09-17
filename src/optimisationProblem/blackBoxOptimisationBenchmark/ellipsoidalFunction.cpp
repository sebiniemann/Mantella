#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/ellipsoidalFunction.hpp>

// C++ standard library
#include <cassert>

namespace mant {
  namespace bbob {
    EllipsoidalFunction::EllipsoidalFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(1000000.0)) {
      setParameterTranslation(getRandomParameterTranslation());
    }

    double EllipsoidalFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      return arma::dot(parameterConditioning_, arma::square(getOscillatedParameter(parameter)));
    }

    std::string EllipsoidalFunction::toString() const {
      return "bbob_ellipsoidal_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> EllipsoidalFunction::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void EllipsoidalFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
