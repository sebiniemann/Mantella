// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("ParticleSwarmOptimisation.optimise", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.optimise]") {
  GIVEN("An optimisation problem") {
    THEN("Optimise the problem") {
      mant::bbob::SphereFunction optimisationProblem(3);
      optimisationProblem.setObjectiveValueTranslation(0.0);

      mant::ParticleSwarmOptimisation optimisationAlgorithm;
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

SCENARIO("ParticleSwarmOptimisation.setMaximalAcceleration", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.setMaximalAcceleration]") {
  GIVEN("A maximal acceleration") {
    WHEN("The maximal acceleration is positive or 0") {
      THEN("Throw no exception") {
        mant::ParticleSwarmOptimisation optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setMaximalAcceleration(1.0));
      }
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.getMaximalAcceleration", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.getMaximalAcceleration]") {
  WHEN("The default maximal acceleration is unchanged") {
    THEN("Return 0") {
      mant::ParticleSwarmOptimisation optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getMaximalAcceleration() == Approx(0.7213475204));
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.setMaximalLocalAttraction", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.setMaximalLocalAttraction]") {
  GIVEN("A maximal local attraction") {
    WHEN("The maximal local attraction is positive or 0") {
      THEN("Throw no exception") {
        mant::ParticleSwarmOptimisation optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setMaximalLocalAttraction(1.0));
      }
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.getMaximalLocalAttraction", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.getMaximalLocalAttraction]") {
  WHEN("The default maximal local attraction is unchanged") {
    THEN("Return 0") {
      mant::ParticleSwarmOptimisation optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getMaximalLocalAttraction() == Approx(1.1931471806));
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.setMaximalGlobalAttraction", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.setMaximalGlobalAttraction]") {
  GIVEN("A maximal global attraction") {
    WHEN("The maximal global attraction is positive or 0") {
      THEN("Throw no exception") {
        mant::ParticleSwarmOptimisation optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setMaximalGlobalAttraction(1.0));
      }
    }
  }
}

SCENARIO("ParticleSwarmOptimisation.getMaximalGlobalAttraction", "[ParticleSwarmOptimisation][ParticleSwarmOptimisation.getMaximalGlobalAttraction]") {
  WHEN("The default maximal global attraction is unchanged") {
    THEN("Return 0") {
      mant::ParticleSwarmOptimisation optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getMaximalGlobalAttraction() == Approx(1.1931471806));
    }
  }
}
