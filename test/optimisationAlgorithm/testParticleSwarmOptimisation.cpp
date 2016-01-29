// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ Standard Library
#include <memory>

// Mantella
#include <mantella>

SCENARIO("ParticleSwarmOptimisation.optimise", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.optimise]") {
  GIVEN("An optimisation problem and a swarm size") {
    WHEN("Called multiple times") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

      mant::ParticleSwarmOptimisation optimisationAlgorithm;
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

  GIVEN("An optimisation problem") {
    WHEN("Called multiple times") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

      mant::ParticleSwarmOptimisation optimisationAlgorithm;
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

SCENARIO("ParticleSwarmOptimisation.setNeighbourhoodTopologyFunction", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.setNeighbourhoodTopologyFunction]") {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;

  GIVEN("A is neighbourhood topology and its name") {
    const std::string& neighbourhoodTopologyFunctionName = "My custom neighbourhood topology function name";
    CAPTURE(neighbourhoodTopologyFunctionName);

    WHEN("The is neighbourhood topology is callable") {
      auto neighbourhoodTopologyFunction = [&optimisationAlgorithm]() {
        return arma::Mat<arma::uword>(0);
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setNeighbourhoodTopologyFunction(neighbourhoodTopologyFunction, neighbourhoodTopologyFunctionName));
      }
    }

    WHEN("The is neighbourhood topology is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setNeighbourhoodTopologyFunction(nullptr, neighbourhoodTopologyFunctionName), std::logic_error);
      }
    }
  }

  GIVEN("A is neighbourhood topology") {
    WHEN("The is neighbourhood topology is callable") {
      auto neighbourhoodTopologyFunction = [&optimisationAlgorithm]() {
        return arma::Mat<arma::uword>(0);
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setNeighbourhoodTopologyFunction(neighbourhoodTopologyFunction));
      }
    }

    WHEN("The is neighbourhood topology is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setNeighbourhoodTopologyFunction(nullptr), std::logic_error);
      }
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.getNeighbourhoodTopologyFunctionName", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.getNeighbourhoodTopologyFunctionName]") {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;

  WHEN("The default neighbourhood topology function is unchanged") {
    THEN("Return 'Random'") {
      CHECK(optimisationAlgorithm.getNeighbourhoodTopologyFunctionName() == "Random");
    }
  }

  WHEN("The default neighbourhood topology function was changed") {
    auto neighbourhoodTopologyFunction = [&optimisationAlgorithm]() {
        return arma::Mat<arma::uword>(0);
    };

    AND_WHEN("A new neighbourhood topology function name was set") {
      const std::string& neighbourhoodTopologyFunctionName = "My custom neighbourhood topology function name";
      CAPTURE(neighbourhoodTopologyFunctionName);

      optimisationAlgorithm.setNeighbourhoodTopologyFunction(neighbourhoodTopologyFunction, neighbourhoodTopologyFunctionName);

      THEN("Return the neighbourhood topology function name") {
        CHECK(optimisationAlgorithm.getNeighbourhoodTopologyFunctionName() == neighbourhoodTopologyFunctionName);
      }
    }

    AND_WHEN("No new neighbourhood topology function name was set") {
      optimisationAlgorithm.setNeighbourhoodTopologyFunction(neighbourhoodTopologyFunction);

      THEN("Return the default, unnamed neighbourhood topology function name") {
        CHECK(optimisationAlgorithm.getNeighbourhoodTopologyFunctionName() == "Unnamed, custom neighbourhood topology function");
      }
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.setMaximalAcceleration", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.setMaximalAcceleration]") {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;

  GIVEN("A maximal acceleration") {
    const double maximalAcceleration = continuousRandomNumber();
    CAPTURE(maximalAcceleration);

    THEN("Throw no exception") {
      CHECK_NOTHROW(optimisationAlgorithm.setMaximalAcceleration(maximalAcceleration));
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.getMaximalAcceleration", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.getMaximalAcceleration]") {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;

  WHEN("The default maximal acceleration is unchanged") {
    THEN("Return 0.0") {
      CHECK(optimisationAlgorithm.getMaximalAcceleration() == Approx(1.0 / (2.0 * std::log(2.0))));
    }
  }

  WHEN("The default maximal acceleration was changed") {
    const double maximalAcceleration = continuousRandomNumber();
    CAPTURE(maximalAcceleration);

    optimisationAlgorithm.setMaximalAcceleration(maximalAcceleration);

    THEN("Return the updated maximal acceleration") {
      CHECK(optimisationAlgorithm.getMaximalAcceleration() == Approx(maximalAcceleration));
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.setMaximalLocalAttraction", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.setMaximalLocalAttraction]") {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;

  GIVEN("A maximal local attraction") {
    const double maximalLocalAttraction = continuousRandomNumber();
    CAPTURE(maximalLocalAttraction);

    THEN("Throw no exception") {
      CHECK_NOTHROW(optimisationAlgorithm.setMaximalLocalAttraction(maximalLocalAttraction));
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.getMaximalLocalAttraction", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.getMaximalLocalAttraction]") {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;

  WHEN("The default maximal local attraction is unchanged") {
    THEN("Return 0.0") {
      CHECK(optimisationAlgorithm.getMaximalLocalAttraction() == Approx(0.5 + std::log(2.0)));
    }
  }

  WHEN("The default maximal local attraction was changed") {
    const double maximalLocalAttraction = continuousRandomNumber();
    CAPTURE(maximalLocalAttraction);

    optimisationAlgorithm.setMaximalLocalAttraction(maximalLocalAttraction);

    THEN("Return the updated maximal local attraction") {
      CHECK(optimisationAlgorithm.getMaximalLocalAttraction() == Approx(maximalLocalAttraction));
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.setMaximalGlobalAttraction", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.setMaximalGlobalAttraction]") {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;

  GIVEN("A maximal global attraction") {
    const double maximalGlobalAttraction = continuousRandomNumber();
    CAPTURE(maximalGlobalAttraction);

    THEN("Throw no exception") {
      CHECK_NOTHROW(optimisationAlgorithm.setMaximalGlobalAttraction(maximalGlobalAttraction));
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.getMaximalGlobalAttraction", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.getMaximalGlobalAttraction]") {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;

  WHEN("The default maximal global attraction is unchanged") {
    THEN("Return 0.0") {
      CHECK(optimisationAlgorithm.getMaximalGlobalAttraction() == Approx(0.5 + std::log(2.0)));
    }
  }

  WHEN("The default maximal global attraction was changed") {
    const double maximalGlobalAttraction = continuousRandomNumber();
    CAPTURE(maximalGlobalAttraction);

    optimisationAlgorithm.setMaximalGlobalAttraction(maximalGlobalAttraction);

    THEN("Return the updated maximal global attraction") {
      CHECK(optimisationAlgorithm.getMaximalGlobalAttraction() == Approx(maximalGlobalAttraction));
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.getNextParametersFunctionName", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.getNextParametersFunctionName]") {
  mant::ParticleSwarmOptimisation optimisationAlgorithm;

  THEN("Return the neighbourhood topology function name") {
    CHECK(optimisationAlgorithm.getNextParametersFunctionName() == "Particle swarm optimisation");
  }
}
