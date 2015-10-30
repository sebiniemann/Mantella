#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/discusFunction.hpp"

// C++ standard library
#include <cassert>

// Mantella
#include "mantella_bits/helper/random.hpp"

namespace mant {
  namespace bbob {
    DiscusFunction::DiscusFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
    }

    double DiscusFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      const arma::Col<double>& z = arma::square(getOscillatedParameter(parameter));
      return 1000000.0 * z(0) + arma::accu(z.tail(z.n_elem - 1));
    }

    std::string DiscusFunction::toString() const {
      return "bbob_discus_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> DiscusFunction::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void DiscusFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
