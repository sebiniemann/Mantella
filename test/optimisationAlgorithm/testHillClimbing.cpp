// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ Standard Library
#include <memory>
#include <random>

// Mantella
#include <mantella>

SCENARIO("HillClimbing.optimise", "[HillClimbing][HillClimbing.optimise]") {
  GIVEN("An optimisation problem") {
    WHEN("Called multiple times") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);
      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

      mant::HillClimbing optimisationAlgorithm;
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

SCENARIO("HillClimbing.setMinimalStepSize", "[HillClimbing][HillClimbing.setMinimalStepSize]") {
  mant::HillClimbing optimisationAlgorithm;

  GIVEN("A minimal step size") {
    WHEN("The minimal step size is not negative (but may be 0)") {
      const double minimalStepSize = std::abs(continuousRandomNumber());
      CAPTURE(minimalStepSize);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setMinimalStepSize(minimalStepSize));
      }
    }

    WHEN("The minimal step size is negative") {
      const double minimalStepSize = -std::abs(continuousRandomNumber()) - 1.0;
      CAPTURE(minimalStepSize);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setMinimalStepSize(minimalStepSize), std::logic_error);
      }
    }
  }
}

SCENARIO("HillClimbing.getMinimalStepSize", "[HillClimbing][HillClimbing.getMinimalStepSize]") {
  mant::HillClimbing optimisationAlgorithm;

  WHEN("The default minimal step size is unchanged") {
    THEN("Return 0.0") {
      CHECK(optimisationAlgorithm.getMinimalStepSize() == Approx(0.0));
    }
  }

  WHEN("The default minimal step size was changed") {
    const double minimalStepSize = std::abs(continuousRandomNumber());
    CAPTURE(minimalStepSize);

    optimisationAlgorithm.setMinimalStepSize(minimalStepSize);

    THEN("Return the updated minimal step size") {
      CHECK(optimisationAlgorithm.getMinimalStepSize() == Approx(minimalStepSize));
    }
  }
}

SCENARIO("HillClimbing.setMaximalStepSize", "[HillClimbing][HillClimbing.setMaximalStepSize]") {
  mant::HillClimbing optimisationAlgorithm;

  GIVEN("A maximal step size") {
    WHEN("The maximal step size is not negative (may not be 0)") {
      const double maximalStepSize = std::abs(continuousRandomNumber()) + 1.0;
      CAPTURE(maximalStepSize);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setMaximalStepSize(maximalStepSize));
      }
    }

    WHEN("The maximal step size is negative or 0") {
      const double maximalStepSize = -std::abs(continuousRandomNumber());
      CAPTURE(maximalStepSize);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setMaximalStepSize(maximalStepSize), std::logic_error);
      }
    }
  }
}

SCENARIO("HillClimbing.getMaximalStepSize", "[HillClimbing][HillClimbing.getMaximalStepSize]") {
  mant::HillClimbing optimisationAlgorithm;

  WHEN("The default maximal step size is unchanged") {
    THEN("Return 0.1") {
      CHECK(optimisationAlgorithm.getMaximalStepSize() == Approx(0.1));
    }
  }

  WHEN("The default maximal step size was changed") {
    const double maximalStepSize = std::abs(continuousRandomNumber()) + 1.0;
    CAPTURE(maximalStepSize);

    optimisationAlgorithm.setMaximalStepSize(maximalStepSize);

    THEN("Return the updated maximal step size") {
      CHECK(optimisationAlgorithm.getMaximalStepSize() == Approx(maximalStepSize));
    }
  }
}

SCENARIO("HillClimbing.getNextParametersFunctionName", "[HillClimbing][HillClimbing.getNextParametersFunctionName]") {
  mant::HillClimbing optimisationAlgorithm;

  THEN("Return the next parameter function name") {
    CHECK(optimisationAlgorithm.getNextParametersFunctionName() == "Hill climbing");
  }
}
