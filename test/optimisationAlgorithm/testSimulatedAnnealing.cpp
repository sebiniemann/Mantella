// Catch
#include <catch.hpp>
#include "../catchHelper.hpp"

SCENARIO("SimulatedAnnealing.optimise", "[SimulatedAnnealing][SimulatedAnnealing.optimise]") {
  GIVEN("An optimisation problem") {
    THEN("Optimise the problem") {
      mant::bbob::SphereFunction optimisationProblem(3);
      optimisationProblem.setObjectiveValueTranslation(0.0);

      mant::SimulatedAnnealing optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(10000);
      optimisationAlgorithm.setAcceptableObjectiveValue(1e-1);

      arma::vec medianUsedNumberOfIterations(100);
      for (arma::uword n = 0; n < medianUsedNumberOfIterations.n_elem; ++n) {
        optimisationAlgorithm.optimise(optimisationProblem);
        medianUsedNumberOfIterations(n) = static_cast<double>(optimisationAlgorithm.getUsedNumberOfIterations());
      }

      // The value is obtained from doubling the median used number of iterations based on 10.000 repetitions.
      CHECK(arma::median(medianUsedNumberOfIterations) <= 200);
    }
  }
}

SCENARIO("SimulatedAnnealing.setMinimalStepSize", "[SimulatedAnnealing][SimulatedAnnealing.setMinimalStepSize]") {
  GIVEN("A minimal step size") {
    WHEN("The minimal step size is positive or 0") {
      THEN("Throw no exception") {
        mant::SimulatedAnnealing optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setMinimalStepSize(1.0));
      }
    }
  }
}

SCENARIO("SimulatedAnnealing.getMinimalStepSize", "[SimulatedAnnealing][SimulatedAnnealing.getMinimalStepSize]") {
  WHEN("The default minimal step size is unchanged") {
    THEN("Return 0") {
      mant::SimulatedAnnealing optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getMinimalStepSize() == Approx(0.0));
    }
  }
}

SCENARIO("SimulatedAnnealing.setMaximalStepSize", "[SimulatedAnnealing][SimulatedAnnealing.setMaximalStepSize]") {
  GIVEN("A maximal step size") {
    WHEN("The maximal step size is positive or 0") {
      THEN("Throw no exception") {
        mant::SimulatedAnnealing optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setMaximalStepSize(1.0));
      }
    }
  }
}

SCENARIO("SimulatedAnnealing.getMaximalStepSize", "[SimulatedAnnealing][SimulatedAnnealing.getMaximalStepSize]") {
  WHEN("The default maximal step size is unchanged") {
    THEN("Return 0") {
      mant::SimulatedAnnealing optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getMaximalStepSize() == Approx(0.1));
    }
  }
}
