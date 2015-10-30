#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/ellipsoidalFunctionRotated.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    EllipsoidalFunctionRotated::EllipsoidalFunctionRotated(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(1000000.0)) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
    }

    double EllipsoidalFunctionRotated::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      return arma::dot(parameterConditioning_, arma::square(getOscillatedParameter(parameter)));
    }

    std::string EllipsoidalFunctionRotated::toString() const {
      return "bbob_ellipsoidal_function_rotated";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> EllipsoidalFunctionRotated::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void EllipsoidalFunctionRotated::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
