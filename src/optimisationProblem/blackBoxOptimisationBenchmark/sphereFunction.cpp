#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/sphereFunction.hpp>

// C++ standard library
#include <cassert>
#include <cmath>

namespace mant {
  namespace bbob {
    SphereFunction::SphereFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions) {
      setParameterTranslation(getRandomParameterTranslation());
    }

    double SphereFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      return std::pow(arma::norm(parameter), 2.0);
    }

    std::string SphereFunction::toString() const {
      return "bbob_sphere_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> SphereFunction::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void SphereFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
