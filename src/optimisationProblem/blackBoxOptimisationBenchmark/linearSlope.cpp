#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/linearSlope.hpp>

// C++ standard library
#include <cassert>
#include <random>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  namespace bbob {
    LinearSlope::LinearSlope(
        const arma::uword numberOfDimensions)
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(10.0)),
        f0_(5.0 * arma::accu(parameterConditioning_)) {
      setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions_, numberOfDimensions_) * (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 1.0 : -1.0));
    }

    double LinearSlope::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);
        
      arma::Col<double> z = parameter;
      z.elem(arma::find(parameter >= 5.0)).fill(5.0);

      return f0_ - arma::dot(parameterConditioning_, z);
    }

    std::string LinearSlope::toString() const {
      return "bbob_linear_slope";
    }
    
#if defined(SUPPORT_MPI)
    std::vector<double> LinearSlope::serialise() const {
      return BlackBoxOptimisationBenchmark::serialise();
    }

    void LinearSlope::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
