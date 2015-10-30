#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/sharpRidgeFunction.hpp>

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    SharpRidgeFunction::SharpRidgeFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    void SharpRidgeFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_,
          "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ),
          "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    double SharpRidgeFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      const arma::Col<double>& z = rotationQ_ * (parameterConditioning_ % parameter);
      return std::pow(z(0), 2.0) + 100.0 * arma::norm(z.tail(z.n_elem - 1));
    }

    std::string SharpRidgeFunction::toString() const {
      return "bbob_sharp_ridge_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> SharpRidgeFunction::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();

      for (arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }

      return serialisedOptimisationProblem;
    }

    void SharpRidgeFunction::deserialise(
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
