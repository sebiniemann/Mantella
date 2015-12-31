#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/schaffersF7FunctionIllConditioned.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/probability.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    SchaffersF7FunctionIllConditioned::SchaffersF7FunctionIllConditioned(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(1000.0))),
          rotationQ_(randomRotationMatrix(numberOfDimensions_)) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(randomRotationMatrix(numberOfDimensions_));

#if defined(SUPPORT_MPI)
      MPI_Bcast(rotationQ_.memptr(), static_cast<int>(rotationQ_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

      setObjectiveFunction([this](
                               const arma::Col<double>& parameter) {
          assert(parameter.n_elem == numberOfDimensions_);
            
          const arma::Col<double>& s = arma::square(parameterConditioning_ % (rotationQ_ * getAsymmetricParameter(0.5, parameter)));
          const arma::Col<double>& z = arma::pow(s.head(s.n_elem - 1) + s.tail(s.n_elem - 1), 0.25);

          return std::pow(arma::mean(z % (1.0 + arma::square(arma::sin(50.0 * arma::pow(z, 0.4))))), 2.0);
      },
          "BBOB Schaffers F7 Function, ill-conditioned");
    }
  }
}
