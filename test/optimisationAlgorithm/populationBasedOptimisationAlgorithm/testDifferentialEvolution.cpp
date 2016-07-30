// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("DifferentialEvolution.optimise", "[DifferentialEvolution][DifferentialEvolution.optimise]") {
  GIVEN("An optimisation problem") {
    THEN("Optimise the problem") {
      mant::bbob::SphereFunction optimisationProblem(3);
      optimisationProblem.setObjectiveValueTranslation(0.0);

      mant::DifferentialEvolution optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(10000);
      optimisationAlgorithm.setAcceptableObjectiveValue(1e-1);

      arma::vec medianUsedNumberOfIterations(100);
      for (arma::uword n = 0; n < medianUsedNumberOfIterations.n_elem; ++n) {
        optimisationAlgorithm.optimise(optimisationProblem);
        medianUsedNumberOfIterations(n) = static_cast<double>(optimisationAlgorithm.getUsedNumberOfIterations());
      }

      // The value is obtained from doubling the median used number of iterations based on 10.000 repetitions.
      CHECK(arma::median(medianUsedNumberOfIterations) <= 500);
    }
  }
}

SCENARIO("DifferentialEvolution.setScalingFactor", "[DifferentialEvolution][DifferentialEvolution.setScalingFactor]") {
  GIVEN("A scaling factor") {
    WHEN("The scaling factor is positive or 0") {
      THEN("Throw no exception") {
        mant::DifferentialEvolution optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setScalingFactor(1.0));
      }
    }
  }
}

SCENARIO("DifferentialEvolution.getScalingFactor", "[DifferentialEvolution][DifferentialEvolution.getScalingFactor]") {
  WHEN("The default scaling factor is unchanged") {
    THEN("Return 0.5") {
      mant::DifferentialEvolution optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getScalingFactor() == Approx(0.5));
    }
  }
}
