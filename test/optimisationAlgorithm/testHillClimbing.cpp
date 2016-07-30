// Catch
#include <catch.hpp>
#include "../catchHelper.hpp"

SCENARIO("HillClimbing.optimise", "[HillClimbing][HillClimbing.optimise]") {
  GIVEN("An optimisation problem") {
    THEN("Optimise the problem") {
      mant::bbob::SphereFunction optimisationProblem(3);
      optimisationProblem.setObjectiveValueTranslation(0.0);

      mant::HillClimbing optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(10000);
      optimisationAlgorithm.setAcceptableObjectiveValue(1e-1);

      arma::vec medianUsedNumberOfIterations(100);
      for (arma::uword n = 0; n < medianUsedNumberOfIterations.n_elem; ++n) {
        optimisationAlgorithm.optimise(optimisationProblem);
        medianUsedNumberOfIterations(n) = static_cast<double>(optimisationAlgorithm.getUsedNumberOfIterations());
      }

      // The value is obtained from doubling the median used number of iterations based on 10.000 repetitions.
      CHECK(arma::median(medianUsedNumberOfIterations) <= 100);
    }
  }
}

SCENARIO("HillClimbing.setMinimalStepSize", "[HillClimbing][HillClimbing.setMinimalStepSize]") {
  GIVEN("A minimal step size") {
    WHEN("The minimal step size is positive or 0") {
      THEN("Throw no exception") {
        mant::HillClimbing optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setMinimalStepSize(1.0));
      }
    }
  }
}

SCENARIO("HillClimbing.getMinimalStepSize", "[HillClimbing][HillClimbing.getMinimalStepSize]") {
  WHEN("The default minimal step size is unchanged") {
    THEN("Return 0") {
      mant::HillClimbing optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getMinimalStepSize() == Approx(0.0));
    }
  }
}

SCENARIO("HillClimbing.setMaximalStepSize", "[HillClimbing][HillClimbing.setMaximalStepSize]") {
  GIVEN("A maximal step size") {
    WHEN("The maximal step size is positive or 0") {
      THEN("Throw no exception") {
        mant::HillClimbing optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setMaximalStepSize(1.0));
      }
    }
  }
}

SCENARIO("HillClimbing.getMaximalStepSize", "[HillClimbing][HillClimbing.getMaximalStepSize]") {
  WHEN("The default maximal step size is unchanged") {
    THEN("Return 0") {
      mant::HillClimbing optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getMaximalStepSize() == Approx(0.1));
    }
  }
}
