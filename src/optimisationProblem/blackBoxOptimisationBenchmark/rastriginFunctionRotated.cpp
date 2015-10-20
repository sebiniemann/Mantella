#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/rastriginFunctionRotated.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include "mantella_bits/helper/assert.hpp"
#include "mantella_bits/helper/random.hpp"

namespace mant {
  namespace bbob {
    RastriginFunctionRotated::RastriginFunctionRotated(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      setParameterTranslation(getRandomParameterTranslation());
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    void RastriginFunctionRotated::setRotationR(
        const arma::Mat<double>& rotationR) {
      verify(rotationR.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    void RastriginFunctionRotated::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    double RastriginFunctionRotated::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      const arma::Col<double>& z = rotationR_ * (parameterConditioning_ % (rotationQ_ * getAsymmetricParameter(0.2, getOscillatedParameter(rotationR_ * parameter))));

      return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0);
    }

    std::string RastriginFunctionRotated::toString() const {
      return "bbob_rastrigin_function_rotated";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> RastriginFunctionRotated::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();

      for (arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }

      for (arma::uword n = 0; n < rotationR_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationR_(n));
      }

      return serialisedOptimisationProblem;
    }

    void RastriginFunctionRotated::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      rotationR_.set_size(numberOfDimensions_, numberOfDimensions_);
      for (arma::uword n = 0; n < rotationR_.n_elem; ++n) {
        rotationR_(n) = serialisedOptimisationProblem.back();
        serialisedOptimisationProblem.pop_back();
      }

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
