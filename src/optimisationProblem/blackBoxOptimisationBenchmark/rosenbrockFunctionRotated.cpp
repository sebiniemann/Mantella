#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/rosenbrockFunctionRotated.hpp>

// C++ standard library
#include <cassert>
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    RosenbrockFunctionRotated::RosenbrockFunctionRotated(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          max_(std::max(1.0, std::sqrt(numberOfDimensions_) / 8.0)) {
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
    }

    double RosenbrockFunctionRotated::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      const arma::Col<double>& z = max_ * parameter + 0.5;

      return 100.0 * std::pow(arma::norm(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1)), 2.0) + std::pow(arma::norm(z.head(z.n_elem - 1) - 1.0), 2.0);
    }

    std::string RosenbrockFunctionRotated::toString() const {
      return "bbob_rosenbrock_function_rotated";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> RosenbrockFunctionRotated::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void RosenbrockFunctionRotated::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
