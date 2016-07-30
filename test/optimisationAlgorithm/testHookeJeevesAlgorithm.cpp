// Catch
#include <catch.hpp>
#include "../catchHelper.hpp"

SCENARIO("HookeJeevesAlgorithm.optimise", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.optimise]") {
  GIVEN("An optimisation problem") {
    THEN("Optimise the problem") {
      mant::bbob::SphereFunction optimisationProblem(3);
      optimisationProblem.setObjectiveValueTranslation(0.0);

      mant::HookeJeevesAlgorithm optimisationAlgorithm;
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

SCENARIO("HookeJeevesAlgorithm.setInitialStepSize", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.setInitialStepSize]") {
  GIVEN("An initial step size") {
    WHEN("The initial step size is positive or 0") {
      THEN("Throw no exception") {
        mant::HookeJeevesAlgorithm optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setInitialStepSize(1.0));
      }
    }
  }
}

SCENARIO("HookeJeevesAlgorithm.getInitialStepSize", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.getInitialStepSize]") {
  WHEN("The default initial step size is unchanged") {
    THEN("Return 1") {
      mant::HookeJeevesAlgorithm optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getInitialStepSize() == Approx(1.0));
    }
  }
}

SCENARIO("HookeJeevesAlgorithm.setStepSizeDecrease", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.setStepSizeDecrease]") {
  GIVEN("A step size decrease") {
    WHEN("The step size decrease is within (0, 1)") {
      THEN("Throw no exception") {
        mant::HookeJeevesAlgorithm optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setStepSizeDecrease(0.5));
      }
    }
  }
}

SCENARIO("HookeJeevesAlgorithm.getStepSizeDecrease", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.getStepSizeDecrease]") {
  WHEN("The default step size decrease is unchanged") {
    THEN("Return 0.5") {
      mant::HookeJeevesAlgorithm optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getStepSizeDecrease() == Approx(0.5));
    }
  }
}
