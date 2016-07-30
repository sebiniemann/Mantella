#include "mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/lunacekBiRastriginFunction.hpp"

// C++ standard library
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/mpi.hpp"
#include "mantella_bits/probability.hpp"
#include "mantella_bits/randomNumberGenerator.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    LunacekBiRastriginFunction::LunacekBiRastriginFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          s_(1.0 - 0.5 / (std::sqrt(static_cast<decltype(s_)>(numberOfDimensions_) + 20.0) - 4.1)),
          mu_(std::sqrt(5.25 / s_)),
          parameterConditinong_(getParameterConditioning(10.0)),
          rotationR_(synchronise(randomRotationMatrix(numberOfDimensions_))),
          rotationQ_(synchronise(randomRotationMatrix(numberOfDimensions_))) {
      assert(numberOfDimensions_ > 1 && "LunacekBiRastriginFunction: The number of dimensions must be greater than 1.");

      if (!isRepresentableAsFloatingPoint(numberOfDimensions_)) {
        throw std::range_error("LunacekBiRastriginFunction: The number of elements must be representable as a floating point.");
      }

      // A vector with all elements randomly and uniformly set to either 2 or -2.
      setParameterScaling(arma::zeros<arma::vec>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 2.0 : -2.0));

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              return std::min(std::pow(arma::norm(parameter_ - 2.5), 2.0), static_cast<double>(numberOfDimensions_) + s_ * std::pow(arma::norm(parameter_ + mu_), 2.0)) + 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * rotationQ_ * (parameterConditinong_ % (rotationR_ * (parameter_ - 2.5))))));
            },
            "BBOB Lunacek bi-Rastrigin Function (f24)"}});
    }
  }
}
