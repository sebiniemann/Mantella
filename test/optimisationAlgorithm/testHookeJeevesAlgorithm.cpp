// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ Standard Library
#include <memory>

// Mantella
#include <mantella>

SCENARIO("HookeJeevesAlgorithm.optimise", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.optimise]") {
  GIVEN("An optimisation problem") {
    WHEN("Called multiple times") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);
      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

      mant::SimulatedAnnealing optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(1);

      THEN("Start with a different parameter") {
        ::mant::isCachingSamples = true;
        for (arma::uword n = 0; n < 100; ++n) {
          optimisationAlgorithm.optimise(optimisationProblem);
        }
        ::mant::isCachingSamples = false;

        CHECK(optimisationProblem.getNumberOfEvaluations() == optimisationProblem.getNumberOfDistinctEvaluations());
      }
    }
  }
}

SCENARIO("HookeJeevesAlgorithm.setInitialStepSize", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.setInitialStepSize]") {
  mant::HookeJeevesAlgorithm optimisationAlgorithm;

  GIVEN("An initial step size") {
    WHEN("The initial step size is not negative") {
      const double initialStepSize = std::abs(continuousRandomNumber()) + 1.0;
      CAPTURE(initialStepSize);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setInitialStepSize(initialStepSize));
      }
    }

    WHEN("The initial step size is negative (including 0)") {
      const double initialStepSize = -std::abs(continuousRandomNumber());
      CAPTURE(initialStepSize);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setInitialStepSize(initialStepSize), std::logic_error);
      }
    }
  }
}

SCENARIO("HookeJeevesAlgorithm.getInitialStepSize", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.getInitialStepSize]") {
  mant::HookeJeevesAlgorithm optimisationAlgorithm;

  WHEN("The default initial step size is unchanged") {
    THEN("Return 0.0") {
      CHECK(optimisationAlgorithm.getInitialStepSize() == Approx(1.0));
    }
  }

  WHEN("The default initial step size was changed") {
    const double initialStepSize = std::abs(continuousRandomNumber()) + 1.0;
    CAPTURE(initialStepSize);

    optimisationAlgorithm.setInitialStepSize(initialStepSize);

    THEN("Return the updated initial step size") {
      CHECK(optimisationAlgorithm.getInitialStepSize() == Approx(initialStepSize));
    }
  }
}

SCENARIO("HookeJeevesAlgorithm.setStepSizeDecrease", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.setStepSizeDecrease]") {
  mant::HookeJeevesAlgorithm optimisationAlgorithm;

  GIVEN("A step size decrease") {
    WHEN("The step size decrease is not negative and less than 1") {
      const double stepSizeDecrease = 1.0 / std::max(2.0, std::abs(continuousRandomNumber()));
      CAPTURE(stepSizeDecrease);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setStepSizeDecrease(stepSizeDecrease));
      }
    }

    WHEN("The step size decrease is negative (including 0)") {
      const double stepSizeDecrease = -std::abs(continuousRandomNumber());
      CAPTURE(stepSizeDecrease);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setStepSizeDecrease(stepSizeDecrease), std::logic_error);
      }
    }

    WHEN("The step size decrease is at least 1") {
      const double stepSizeDecrease = std::abs(continuousRandomNumber()) + 1.0;
      CAPTURE(stepSizeDecrease);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setStepSizeDecrease(stepSizeDecrease), std::logic_error);
      }
    }
  }
}

SCENARIO("HookeJeevesAlgorithm.getStepSizeDecrease", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.getStepSizeDecrease]") {
  mant::HookeJeevesAlgorithm optimisationAlgorithm;

  WHEN("The default step size decrease is unchanged") {
    THEN("Return 0.0") {
      CHECK(optimisationAlgorithm.getStepSizeDecrease() == Approx(0.5));
    }
  }

  WHEN("The default step size decrease was changed") {
    const double stepSizeDecrease = 1.0 / std::max(2.0, std::abs(continuousRandomNumber()));
    CAPTURE(stepSizeDecrease);

    optimisationAlgorithm.setStepSizeDecrease(stepSizeDecrease);

    THEN("Return the updated step size decrease") {
      CHECK(optimisationAlgorithm.getStepSizeDecrease() == Approx(stepSizeDecrease));
    }
  }
}

SCENARIO("HookeJeevesAlgorithm.getNextParametersFunctionName", "[HookeJeevesAlgorithm][HookeJeevesAlgorithm.getNextParametersFunctionName]") {
  mant::HookeJeevesAlgorithm optimisationAlgorithm;

  THEN("Return the next parameter function name") {
    CHECK(optimisationAlgorithm.getNextParametersFunctionName() == "Hooke-Jeeves algorithm");
  }
}
