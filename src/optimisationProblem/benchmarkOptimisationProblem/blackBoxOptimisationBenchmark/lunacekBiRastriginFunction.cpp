#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/lunacekBiRastriginFunction.hpp"

// C++ standard library
#include <cassert>
#include <random>
#include <cmath>
#include <algorithm>

// Mantella
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
          s_(1.0 - 0.5 / (std::sqrt(static_cast<double>(numberOfDimensions_) + 20.0) - 4.1)),
          mu_(std::sqrt(5.25 / s_)),
          parameterConditinong_(getParameterConditioning(10.0)),
          rotationR_(randomRotationMatrix(numberOfDimensions_)),
          rotationQ_(randomRotationMatrix(numberOfDimensions_)) {
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      setParameterScaling(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 2.0 : -2.0));

#if defined(SUPPORT_MPI)
      MPI_Bcast(rotationR_.memptr(), static_cast<int>(rotationR_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(rotationQ_.memptr(), static_cast<int>(rotationQ_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

      setObjectiveFunction([this](
                               const arma::Col<double>& parameter) {
          assert(parameter.n_elem == numberOfDimensions_);
            
          return std::min(std::pow(arma::norm(parameter - 2.5), 2.0), static_cast<double>(numberOfDimensions_) + s_ * std::pow(arma::norm(parameter + mu_), 2.0)) + 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * rotationQ_ * (parameterConditinong_ % (rotationR_ * (parameter - 2.5))))));
      },
          "BBOB Lunacek bi-Rastrigin Function");
    }
  }
}
