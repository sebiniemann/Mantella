#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/stepEllipsoidalFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>
#include <algorithm>

// Mantella
#include "mantella_bits/helper/assert.hpp"
#include "mantella_bits/helper/random.hpp"

namespace mant {
  namespace bbob {
    StepEllipsoidalFunction::StepEllipsoidalFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          firstParameterConditioning_(getParameterConditioning(std::sqrt(10.0))),
          secondParameterConditioning_(getParameterConditioning(100.0)) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    void StepEllipsoidalFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    double StepEllipsoidalFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      const arma::Col<double>& s = firstParameterConditioning_ % parameter;

      arma::Col<double> z = s;
      for (arma::uword n = 0; n < z.n_elem; ++n) {
        const double value = s(n);

        if (std::abs(value) > 0.5) {
          z(n) = std::round(value);
        } else {
          z(n) = std::round(value * 10.0) / 10.0;
        }
      }

      return 0.1 * std::max(std::abs(s(0)) / 10000.0, arma::dot(secondParameterConditioning_, arma::square(rotationQ_ * z)));
    }

    std::string StepEllipsoidalFunction::toString() const {
      return "bbob_step_ellipsoidal_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> StepEllipsoidalFunction::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();

      for (arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }

      return serialisedOptimisationProblem;
    }

    void StepEllipsoidalFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      rotationQ_.set_size(numberOfDimensions_, numberOfDimensions_);
      for (arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = serialisedOptimisationProblem.back();
        serialisedOptimisationProblem.pop_back();
      }

      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
