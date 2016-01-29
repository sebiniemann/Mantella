// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("SimulatedAnnealing.optimise", "[SimulatedAnnealing][SimulatedAnnealing.optimise]") {
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

SCENARIO("SimulatedAnnealing.setIsAcceptableStateFunction", "[SimulatedAnnealing][SimulatedAnnealing.setIsAcceptableStateFunction]") {
  mant::SimulatedAnnealing optimisationAlgorithm;

  GIVEN("A is acceptable state function and its name") {
    const std::string& isAcceptableStateFunctionName = "My custom is acceptable state function name";
    CAPTURE(isAcceptableStateFunctionName);

    WHEN("The is acceptable state function is callable") {
      auto isAcceptableStateFunction = [&optimisationAlgorithm](
          const double objectiveValue) {
        return true;
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setIsAcceptableStateFunction(isAcceptableStateFunction, isAcceptableStateFunctionName));
      }
    }

    WHEN("The is acceptable state function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setIsAcceptableStateFunction(nullptr, isAcceptableStateFunctionName), std::logic_error);
      }
    }
  }

  GIVEN("A is acceptable state function") {
    WHEN("The is acceptable state function is callable") {
      auto isAcceptableStateFunction = [&optimisationAlgorithm](
          const double objectiveValue) {
        return true;
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setIsAcceptableStateFunction(isAcceptableStateFunction));
      }
    }

    WHEN("The is acceptable state function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setIsAcceptableStateFunction(nullptr), std::logic_error);
      }
    }
  }
}

SCENARIO("SimulatedAnnealing.getIsAcceptableStateFunctionName", "[SimulatedAnnealing][SimulatedAnnealing.getIsAcceptableStateFunctionName]") {
  mant::SimulatedAnnealing optimisationAlgorithm;

  WHEN("The default is acceptable state function is unchanged") {
    THEN("Return 'Exponential'") {
      CHECK(optimisationAlgorithm.getIsAcceptableStateFunctionName() == "Exponential");
    }
  }

  WHEN("The default is acceptable state function was changed") {
    auto isAcceptableStateFunction = [&optimisationAlgorithm](
        const double objectiveValue) {
      return true;
    };

    AND_WHEN("A new is acceptable state function name was set") {
      const std::string& isAcceptableStateFunctionName = "My custom is acceptable state function name";
      CAPTURE(isAcceptableStateFunctionName);

      optimisationAlgorithm.setIsAcceptableStateFunction(isAcceptableStateFunction, isAcceptableStateFunctionName);

      THEN("Return the next parameter function name") {
        CHECK(optimisationAlgorithm.getIsAcceptableStateFunctionName() == isAcceptableStateFunctionName);
      }
    }

    AND_WHEN("No is acceptable state function name was set") {
      optimisationAlgorithm.setIsAcceptableStateFunction(isAcceptableStateFunction);

      THEN("Return the default, unnamed is acceptable state function name") {
        CHECK(optimisationAlgorithm.getIsAcceptableStateFunctionName() == "Unnamed, custom is acceptable state function");
      }
    }
  }
}

SCENARIO("SimulatedAnnealing.setMinimalStepSize", "[SimulatedAnnealing][SimulatedAnnealing.setMinimalStepSize]") {
  mant::SimulatedAnnealing optimisationAlgorithm;

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

SCENARIO("SimulatedAnnealing.getMinimalStepSize", "[SimulatedAnnealing][SimulatedAnnealing.getMinimalStepSize]") {
  mant::SimulatedAnnealing optimisationAlgorithm;

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

SCENARIO("SimulatedAnnealing.setMaximalStepSize", "[SimulatedAnnealing][SimulatedAnnealing.setMaximalStepSize]") {
  mant::SimulatedAnnealing optimisationAlgorithm;

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

SCENARIO("SimulatedAnnealing.getMaximalStepSize", "[SimulatedAnnealing][SimulatedAnnealing.getMaximalStepSize]") {
  mant::SimulatedAnnealing optimisationAlgorithm;

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

SCENARIO("SimulatedAnnealing.getNextParametersFunctionName", "[SimulatedAnnealing][SimulatedAnnealing.getNextParametersFunctionName]") {
  mant::SimulatedAnnealing optimisationAlgorithm;

  THEN("Return the next parameter function name") {
    CHECK(optimisationAlgorithm.getNextParametersFunctionName() == "Simulated annealing");
  }
}
