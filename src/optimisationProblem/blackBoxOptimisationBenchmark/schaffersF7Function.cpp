#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/schaffersF7Function.hpp>

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    SchaffersF7Function::SchaffersF7Function(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    void SchaffersF7Function::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_,
          "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ),
          "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    double SchaffersF7Function::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);

      const arma::Col<double>& s = arma::square(parameterConditioning_ % (rotationQ_ * getAsymmetricParameter(0.5, parameter)));
      const arma::Col<double>& z = arma::pow(s.head(s.n_elem - 1) + s.tail(s.n_elem - 1), 0.25);

      return std::pow(arma::mean(z % (1.0 + arma::square(arma::sin(50.0 * arma::pow(z, 0.4))))), 2.0);
    }

    std::string SchaffersF7Function::toString() const {
      return "bbob_schaffers_f7_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> SchaffersF7Function::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();

      for (arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }

      return serialisedOptimisationProblem;
    }

    void SchaffersF7Function::deserialise(
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
