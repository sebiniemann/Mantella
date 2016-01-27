#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/bentCigarFunction.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <cassert>

// Mantella
#include "mantella_bits/probability.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    BentCigarFunction::BentCigarFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          rotationQ_(randomRotationMatrix(numberOfDimensions_)) {
      setParameterTranslation(getRandomParameterTranslation());

#if defined(SUPPORT_MPI)
      MPI_Bcast(rotationQ_.memptr(), static_cast<int>(rotationQ_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

      setObjectiveFunction(
          [this](
              const arma::Col<double>& parameter_) {
            assert(parameter_.n_elem == numberOfDimensions_);
              
            const arma::Col<double>& z = arma::square(rotationQ_ * getAsymmetricParameter(0.5, rotationQ_ * parameter_));
            return z(0) + 1000000.0 * arma::accu(z.tail(z.n_elem - 1));
          },
          "BBOB Bent Cigar Function (f12)");
    }
  }
}
