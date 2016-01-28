// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

extern std::string testDirectory;

SCENARIO("fitnessDistanceCorrelation", "[samplesAnalysis][fitnessDistanceCorrelation]") {
  GIVEN("A set of samples") {
    arma::Mat<double> parameters;
    REQUIRE(parameters.load(::rootTestDataDirectory + "/_parameters_3x100.input"));

    arma::Row<double> objectiveValues;
    REQUIRE(objectiveValues.load(::rootTestDataDirectory + "/_objectiveValues_1x100.input"));

    std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      samples.insert({parameters.col(n), objectiveValues(n)});
    }

    WHEN("The set is dimensionally consistent") {
      THEN("Return the fitness distance correlation") {
        CHECK(mant::fitnessDistanceCorrelation(samples) == Approx(-0.027167769070106));
      }
    }

    WHEN("The set is dimensionally inconsistent") {
      samples.insert({getContinuousRandomNumbers(parameters.n_rows + 1), getContinuousRandomNumber()});

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::fitnessDistanceCorrelation(samples), std::logic_error);
      }
    }

    WHEN("The number of samples is less than 2") {
      samples.clear();
      samples.insert({getContinuousRandomNumbers(parameters.n_rows), getContinuousRandomNumber()});

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::fitnessDistanceCorrelation(samples), std::logic_error);
      }
    }
  }
}

SCENARIO("lipschitzContinuity", "[samplesAnalysis][lipschitzContinuity]") {
  GIVEN("A set of samples") {
    arma::Mat<double> parameters;
    REQUIRE(parameters.load(::rootTestDataDirectory + "/_parameters_3x100.input"));

    arma::Row<double> objectiveValues;
    REQUIRE(objectiveValues.load(::rootTestDataDirectory + "/_objectiveValues_1x100.input"));

    std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      samples.insert({parameters.col(n), objectiveValues(n)});
    }

    WHEN("The set is dimensionally consistent") {
      THEN("Return the Lipschitz continuity") {
        CHECK(mant::lipschitzContinuity(samples) == Approx(60.215382023317567));
      }
    }

    WHEN("The set is dimensionally inconsistent") {
      samples.insert({getContinuousRandomNumbers(parameters.n_rows + 1), getContinuousRandomNumber()});

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::lipschitzContinuity(samples), std::logic_error);
      }
    }

    WHEN("The number of samples is less than 2") {
      samples.clear();
      samples.insert({getContinuousRandomNumbers(parameters.n_rows), getContinuousRandomNumber()});

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::lipschitzContinuity(samples), std::logic_error);
      }
    }
  }
}
