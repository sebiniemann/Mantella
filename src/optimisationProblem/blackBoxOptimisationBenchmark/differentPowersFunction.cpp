#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/differentPowersFunction.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    DifferentPowersFunction::DifferentPowersFunction(
        const arma::uword numberOfDimensions)
      : BlackBoxOptimisationBenchmark(numberOfDimensions) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
    }

    double DifferentPowersFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);
        
      const arma::Col<double>& z = arma::abs(parameter);
      return arma::norm(z % getConditionedParameter(arma::square(z)));
    }

    std::string DifferentPowersFunction::toString() const {
      return "bbob_different_powers_function";
    }
    
#if defined(SUPPORT_MPI)
    std::vector<double> DifferentPowersFunction::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void DifferentPowersFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
