// Catch
#include <catch.hpp>
#include "../catchHelper.hpp"

SCENARIO("bbob::BlackBoxOptimisationBenchmark::BlackBoxOptimisationBenchmark", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark::BlackBoxOptimisationBenchmark]") {
  GIVEN("A number of dimensions") {
    WHEN("The number of dimensions is greater than 0") {
      THEN("Throw no exception") {
        CHECK_NOTHROW(mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(2));
      }
    }
  }
}

SCENARIO("bbob::BlackBoxOptimisationBenchmark.getLowerBounds", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark.getLowerBounds]") {
  THEN("Return the (-5, ..., -5)") {
    mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(2);
    CHECK(arma::approx_equal(optimisationProblem.getLowerBounds(), arma::vec({-5.0, -5.0}), "absdiff", ::mant::machinePrecision) == true);
  }
}

SCENARIO("bbob::BlackBoxOptimisationBenchmark.getUpperBounds", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark.getUpperBounds]") {
  THEN("Return the default upper bounds (5, ..., 5)") {
    mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(2);
    CHECK(arma::approx_equal(optimisationProblem.getUpperBounds(), arma::vec({5.0, 5.0}), "absdiff", ::mant::machinePrecision) == true);
  }
}

SCENARIO("bbob::BlackBoxOptimisationBenchmark.getObjectiveValueTranslation", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark.getObjectiveValueTranslation]") {
  THEN("Return an objective value translation within [-1000, 1000], based on a Cauchy distribution with location 0.0 and scale 100.0") {
    arma::mat::fixed<10000, 1> objectiveValueTranslations;
    for (arma::uword n = 0; n < objectiveValueTranslations.n_elem; ++n) {
      mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(2);
      objectiveValueTranslations(n) = optimisationProblem.getObjectiveValueTranslation();
    }

    CHECK(objectiveValueTranslations.min() >= -1000.0);
    CHECK(objectiveValueTranslations.max() <= 1000.0);
    CHECK(isCauchyDistributed(objectiveValueTranslations, 100.0) == true);
  }
}
