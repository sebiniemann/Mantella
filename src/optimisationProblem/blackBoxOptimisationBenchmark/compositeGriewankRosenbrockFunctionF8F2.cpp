#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/compositeGriewankRosenbrockFunctionF8F2.hpp>

// C++ standard library
#include <cassert>
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    CompositeGriewankRosenbrockFunctionF8F2::CompositeGriewankRosenbrockFunctionF8F2(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          max_(std::max(1.0, std::sqrt(numberOfDimensions_) / 8.0)) {
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
    }

    double CompositeGriewankRosenbrockFunctionF8F2::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      const arma::Col<double>& s = max_ * parameter + 0.5;
      const arma::Col<double>& z = 100.0 * arma::square(arma::square(s.head(s.n_elem - 1)) - s.tail(s.n_elem - 1)) + arma::square(s.head(s.n_elem - 1) - 1.0);

      return 10.0 * (arma::mean(z / 4000.0 - arma::cos(z)) + 1.0);
    }

    std::string CompositeGriewankRosenbrockFunctionF8F2::toString() const {
      return "bbob_composite_griewank_rosenbrock_function_f8f2";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> CompositeGriewankRosenbrockFunctionF8F2::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void CompositeGriewankRosenbrockFunctionF8F2::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
