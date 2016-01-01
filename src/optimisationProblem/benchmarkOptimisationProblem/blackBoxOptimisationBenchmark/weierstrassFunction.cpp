#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/weierstrassFunction.hpp"

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
    WeierstrassFunction::WeierstrassFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          parameterConditioning_(getParameterConditioning(std::sqrt(0.01))),
          rotationR_(randomRotationMatrix(numberOfDimensions_)),
          rotationQ_(randomRotationMatrix(numberOfDimensions_)) {
      setParameterTranslation(getRandomParameterTranslation());

#if defined(SUPPORT_MPI)
      MPI_Bcast(rotationR_.memptr(), static_cast<int>(rotationR_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(rotationQ_.memptr(), static_cast<int>(rotationQ_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

      setObjectiveFunction(
          [this](
              const arma::Col<double>& parameter) {
            assert(parameter.n_elem == numberOfDimensions_);
              
            const arma::Col<double>& z = rotationR_ * (parameterConditioning_ % (rotationQ_ * getOscillatedParameter(rotationR_ * parameter)));

            double sum = 0.0;
            for (arma::uword n = 0; n < z.n_elem; ++n) {
              for (arma::uword k = 0; k < 12; ++k) {
                sum += std::pow(0.5, k) * std::cos(2.0 * arma::datum::pi * std::pow(3.0, k) * (z(n) + 0.5));
              }
            }

            return 10.0 * std::pow(sum / static_cast<double>(numberOfDimensions_) +1.99951171875, 3.0);
          },
          "BBOB Weierstrass Function");
    }
  }
}
