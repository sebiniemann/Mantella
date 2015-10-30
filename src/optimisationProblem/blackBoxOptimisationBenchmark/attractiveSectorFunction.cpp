#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/attractiveSectorFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include "mantella_bits/helper/assert.hpp"
#include "mantella_bits/helper/random.hpp"

namespace mant {
  namespace bbob {
    AttractiveSectorFunction::AttractiveSectorFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    void AttractiveSectorFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    double AttractiveSectorFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      arma::Col<double> z = rotationQ_ * (parameterConditioning_ % parameter);
      z.elem(arma::find(z % parameterTranslation_ > 0.0)) *= 100.0;

      return std::pow(getOscillatedValue(std::pow(arma::norm(z), 2.0)), 0.9);
    }

    std::string AttractiveSectorFunction::toString() const {
      return "bbob_attractive_sector_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> AttractiveSectorFunction::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();

      for (arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }

      return serialisedOptimisationProblem;
    }

    void AttractiveSectorFunction::deserialise(
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
