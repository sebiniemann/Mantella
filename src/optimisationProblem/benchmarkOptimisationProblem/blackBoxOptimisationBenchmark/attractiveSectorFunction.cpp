#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/attractiveSectorFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>

// Mantella
#include "mantella_bits/probability.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    AttractiveSectorFunction::AttractiveSectorFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(10.0))),
          rotationQ_(randomRotationMatrix(numberOfDimensions_)) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(randomRotationMatrix(numberOfDimensions_));

#if defined(SUPPORT_MPI)
      MPI_Bcast(rotationQ_.memptr(), static_cast<int>(rotationQ_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

      setObjectiveFunction(
          [this](
              const arma::Col<double>& parameter) {
            assert(parameter.n_elem == numberOfDimensions_);
              
            arma::Col<double> z = rotationQ_ * (parameterConditioning_ % parameter);
            z.elem(arma::find(z % parameterTranslation_ > 0.0)) *= 100.0;

            return std::pow(getOscillatedValue(std::pow(arma::norm(z), 2.0)), 0.9);
          },
          "BBOB Attractive Sector Function");
    }
  }
}
