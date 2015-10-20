#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/weierstrassFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include "mantella_bits/helper/assert.hpp"
#include "mantella_bits/helper/random.hpp"

namespace mant {
  namespace bbob {
    WeierstrassFunction::WeierstrassFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          f0_(-1.99951171875),
          parameterConditioning_(getParameterConditioning(std::sqrt(0.01))) {
      setParameterTranslation(getRandomParameterTranslation());
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    void WeierstrassFunction::setRotationR(
        const arma::Mat<double>& rotationR) {
      verify(rotationR.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    void WeierstrassFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    double WeierstrassFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      const arma::Col<double>& z = rotationR_ * (parameterConditioning_ % (rotationQ_ * getOscillatedParameter(rotationR_ * parameter)));

      double sum = 0.0;
      for (arma::uword n = 0; n < z.n_elem; ++n) {
        for (arma::uword k = 0; k < 12; ++k) {
          sum += std::pow(0.5, k) * std::cos(2.0 * arma::datum::pi * std::pow(3.0, k) * (z(n) + 0.5));
        }
      }

      return 10.0 * std::pow(sum / static_cast<double>(numberOfDimensions_) - f0_, 3.0);
    }

    std::string WeierstrassFunction::toString() const {
      return "bbob_weierstrass_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> WeierstrassFunction::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();

      for (arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }

      for (arma::uword n = 0; n < rotationR_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationR_(n));
      }

      return serialisedOptimisationProblem;
    }

    void WeierstrassFunction::deserialise(
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
