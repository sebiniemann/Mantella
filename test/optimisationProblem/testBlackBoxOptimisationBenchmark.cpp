// Catch
#include <catch.hpp>
#include "../catchHelper.hpp"

SCENARIO("bbob::BlackBoxOptimisationBenchmark::BlackBoxOptimisationBenchmark", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark::BlackBoxOptimisationBenchmark]") {
  GIVEN("A number of dimensions") {
    WHEN("The number of dimensions is 0") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(0), std::domain_error);
      }
    }

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
    arma::mat::fixed<10000, 1> getObjectiveValueTranslations;
    for (arma::uword n = 0; n < getObjectiveValueTranslations.n_elem; ++n) {
      mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(2);
      getObjectiveValueTranslations(n) = optimisationProblem.getObjectiveValueTranslation();
    }

    arma::mat histogram = arma::conv_to<arma::mat>::from(arma::umat(arma::histc(getObjectiveValueTranslations, arma::linspace<arma::vec>(-1000.0, 1000.0, 11))).head_rows(10));

    // Transforms the limited Cauchy distributed histograms to a uniformly distributed one.
    histogram.each_col() %= arma::vec({28.248587570621467, 86.206896551724142, 39.525691699604742, 13.793103448275863, 2.840909090909091, 2.824060999717594, 13.888888888888889, 37.593984962406019, 76.923076923076920, 26.666666666666668});

    CHECK(isUniformlyDistributed(histogram, -1000.0, 1000.0) == true);
  }
}
