#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/lunacekBiRastriginFunction.hpp>

// C++ standard library
#include <cassert>
#include <random>
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/rng.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    LunacekBiRastriginFunction::LunacekBiRastriginFunction(
        const arma::uword numberOfDimensions)
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        s_(1.0 - 0.5 / (std::sqrt(static_cast<double>(numberOfDimensions_) + 20.0) - 4.1)),
        mu_(std::sqrt(5.25 / s_)),
        parameterConditinong_(getParameterConditioning(10.0)) {
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      setParameterScaling(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 2.0 : -2.0));
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    void LunacekBiRastriginFunction::setRotationR(
        const arma::Mat<double>& rotationR) {
      verify(rotationR.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    void LunacekBiRastriginFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    double LunacekBiRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);
        
      return std::min(std::pow(arma::norm(parameter - 2.5), 2.0), static_cast<double>(numberOfDimensions_) + s_ * std::pow(arma::norm(parameter + mu_), 2.0)) + 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * rotationQ_ * (parameterConditinong_ % (rotationR_ * (parameter - 2.5))))));
    }

    std::string LunacekBiRastriginFunction::toString() const {
      return "bbob_lunacek_bi_rastrigin_function";
    }
    
#if defined(SUPPORT_MPI)
    std::vector<double> LunacekBiRastriginFunction::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();
      
      for(arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }
      
      return serialisedOptimisationProblem;
    }

    void LunacekBiRastriginFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      rotationQ_.set_size(numberOfDimensions_, numberOfDimensions_);
      for(arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = serialisedOptimisationProblem.back();
        serialisedOptimisationProblem.pop_back();
      }
        
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
