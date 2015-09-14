#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/rosenbrockFunction.hpp>

// C++ standard library
#include <cassert>
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {
  namespace bbob {
    RosenbrockFunction::RosenbrockFunction(
        const arma::uword numberOfDimensions)
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        max_(std::max(1.0, std::sqrt(numberOfDimensions_) / 8.0)) {
      setParameterTranslation(0.75 * getRandomParameterTranslation());
    }

    double RosenbrockFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);
        
      const arma::Col<double>& z = max_ * parameter + 1.0;

      return 100.0 * std::pow(arma::norm(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1)), 2.0) + std::pow(arma::norm(z.head(z.n_elem - 1) - 1.0), 2.0);
    }

    std::string RosenbrockFunction::toString() const {
      return "bbob_rosenbrock_function";
    }
    
#if defined(SUPPORT_MPI)
    std::vector<double> RosenbrockFunction::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void RosenbrockFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
