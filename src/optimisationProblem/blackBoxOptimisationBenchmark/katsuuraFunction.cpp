#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/katsuuraFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include "mantella_bits/helper/assert.hpp"
#include "mantella_bits/helper/random.hpp"

namespace mant {
  namespace bbob {
    KatsuuraFunction::KatsuuraFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(10.0)) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    void KatsuuraFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    double KatsuuraFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      arma::Col<double> z = rotationQ_ * (parameterConditioning_ % parameter);

      double product = 1.0;
      for (arma::uword n = 0; n < z.n_elem; ++n) {
        const double value = z(n);

        double sum = 0.0;
        for (arma::uword k = 1; k < 33; ++k) {
          const double power = std::pow(2.0, k);
          sum += std::abs(power * value - std::round(power * value)) / power;
        }

        product *= std::pow(1.0 + (static_cast<double>(n) + 1.0) * sum, 10.0 / std::pow(numberOfDimensions_, 1.2));
      }

      return 10.0 / std::pow(numberOfDimensions_, 2.0) * (product - 1.0);
    }

    std::string KatsuuraFunction::toString() const {
      return "bbob_katsuura_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> KatsuuraFunction::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();

      for (arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }

      return serialisedOptimisationProblem;
    }

    void KatsuuraFunction::deserialise(
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
