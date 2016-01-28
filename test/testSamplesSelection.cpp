// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("elitists", "[samplesSelection][elitists]") {
  GIVEN("A set of samples and a number of samples to select") {
    arma::Mat<double> parameters;
    REQUIRE(parameters.load(::rootTestDataDirectory + "/_parameters_3x100.input"));

    arma::Row<double> objectiveValues;
    REQUIRE(objectiveValues.load(::rootTestDataDirectory + "/_objectiveValues_1x100.input"));

    std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      samples.insert({parameters.col(n), objectiveValues(n)});
    }

    WHEN("The set is dimensionally consistent and the number of samples to select is not larger than the number of samples") {
      const arma::uword numberOfSamplesToSelect = 10;
      CAPTURE(numberOfSamplesToSelect);

      THEN("Return the elitists") {
        arma::Row<arma::uword> expectedSamplesIndex;
        REQUIRE(expectedSamplesIndex.load(::rootTestDataDirectory + "/elitists_1x10.expected"));

        std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> expectedSamples;
        for (arma::uword n = 0; n < expectedSamplesIndex.n_elem; ++n) {
          expectedSamples.insert({parameters.col(expectedSamplesIndex(n)), objectiveValues(expectedSamplesIndex(n))});
        }

        HAS_SAME_SAMPLES(mant::elitists(samples, numberOfSamplesToSelect), expectedSamples);
      }
    }

    WHEN("The set is dimensionally inconsistent") {
      const arma::uword numberOfSamplesToSelect = 10;
      CAPTURE(numberOfSamplesToSelect);

      samples.insert({getContinuousRandomNumbers(parameters.n_rows + 1), getContinuousRandomNumber()});

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::elitists(samples, numberOfSamplesToSelect), std::logic_error);
      }
    }

    WHEN("The number of samples to select is larger than the number of samples") {
      const arma::uword numberOfSamplesToSelect = samples.size() + 1;
      CAPTURE(numberOfSamplesToSelect);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::elitists(samples, numberOfSamplesToSelect), std::logic_error);
      }
    }
  }
}
