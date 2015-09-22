// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <cmath>

// Mantella
#include <mantella>

TEST_CASE("getOrdinaryLeastSquaresEstimate") {
  SECTION("Returns the ordinary least squares estimates.") {
  }

  SECTION("Returns the exact coefficients for a linear function.") {
    const arma::uword numberOfDimensions = getRandomNumberOfValues();
    CAPTURE(numberOfDimensions);

    const arma::uword numberOfParameters = numberOfDimensions + getRandomNumberOfValues();
    CAPTURE(numberOfParameters);
    
    arma::Mat<double> parameters(numberOfDimensions + 1, numberOfParameters);
    parameters.head_rows(numberOfDimensions) = getRandomValues(numberOfDimensions, numberOfParameters);
    parameters.row(numberOfDimensions).ones();

    const arma::Col<double>& coefficients = getRandomValues(numberOfDimensions + 1);
    CAPTURE(coefficients);
    
    arma::Col<double> objectiveValues(numberOfParameters);
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      objectiveValues(n) = arma::dot(parameters.col(n), coefficients);
    }
    CAPTURE(objectiveValues);
    
    IS_EQUAL(mant::getOrdinaryLeastSquaresEstimate(parameters, objectiveValues), coefficients);
  }

  SECTION("Exception tests") {
    SECTION("Throws an exception, if the number of parameters is not equal to the number of objective values.") {
      const arma::uword numberOfDimensions = getRandomNumberOfValues();
      CAPTURE(numberOfDimensions);

      const arma::uword numberOfParameters = getRandomNumberOfValues();
      CAPTURE(numberOfParameters);
      const arma::Mat<double>& parameters = getRandomValues(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);

      const arma::uword numberOfObjectiveValues = getDifferentRandomNumberOfValues(numberOfParameters);
      CAPTURE(numberOfObjectiveValues);
      const arma::Col<double>& objectiveValues = getRandomValues(numberOfObjectiveValues);
      CAPTURE(objectiveValues);

      CHECK_THROWS_AS(mant::getOrdinaryLeastSquaresEstimate(parameters, objectiveValues), std::logic_error);
    }
  }
}

// TEST_CASE("getGeneralisedLeastSquaresEstimate") {
  // SECTION("Returns the generalised least squares estimates.") {
  // }

  // SECTION(
      // "Returns the exact coefficients for a linear function.") {
    // const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    // CAPTURE(numberOfDimensions);

    // const arma::uword numberOfParameters = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    // CAPTURE(numberOfParameters);
    // const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
    // CAPTURE(parameters);

    // const arma::Col<double>& coefficients = arma::randu<arma::Col<double>>(numberOfDimensions + 1) * 200 - 100;
    // CAPTURE(coefficients);
    
    // const arma::Col<double>& objectiveValues = parameters * coefficients.head(numberOfDimensions) + parameters(numberOfDimensions - 1);
    // CAPTURE(objectiveValues);

    // IS_EQUAL(mant::getOrdinaryLeastSquaresEstimate(parameters, objectiveValues), coefficients);
  // }

  // SECTION("Exception tests") {
    // SECTION("Throws an exception, if the number of parameters is not equal to the number of objective values.") {
      // const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      // CAPTURE(numberOfDimensions);

      // const arma::uword numberOfParameters = std::uniform_int_distribution<arma::uword>(5, 10)(mant::Rng::getGenerator());
      // CAPTURE(numberOfParameters);
      // const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
      // CAPTURE(parameters);

      // const arma::uword numberOfObjectiveValues = static_cast<arma::uword>(static_cast<int>(numberOfParameters) + (std::bernoulli_distribution(0.5)(mant::Rng::getGenerator()) ? 1 : -1) * std::uniform_int_distribution<int>(1, 4)(mant::Rng::getGenerator()));
      // CAPTURE(numberOfObjectiveValues);
      // const arma::Col<double>& objectiveValues = arma::randu<arma::Col<double>>(numberOfParameters) * 200 - 100;
      // CAPTURE(objectiveValues);

      // const arma::Mat<double>& variance = arma::randu<arma::Mat<double>>(numberOfParameters, numberOfParameters) * 200 - 100;
      // CAPTURE(variance);

      // CHECK_THROWS_AS(mant::getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, variance), std::logic_error);
    // }

    // SECTION("Throws an exception, if the variance matrix is not square.") {
      // const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      // CAPTURE(numberOfDimensions);

      // const arma::uword numberOfParameters = std::uniform_int_distribution<arma::uword>(5, 10)(mant::Rng::getGenerator());
      // CAPTURE(numberOfParameters);
      // const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
      // CAPTURE(parameters);

      // const arma::uword numberOfObjectiveValues = static_cast<arma::uword>(static_cast<int>(numberOfParameters) + (std::bernoulli_distribution(0.5)(mant::Rng::getGenerator()) ? 1 : -1) * std::uniform_int_distribution<arma::uword>(1, 4)(mant::Rng::getGenerator()));
      // CAPTURE(numberOfObjectiveValues);
      // const arma::Col<double>& objectiveValues = arma::randu<arma::Col<double>>(numberOfParameters) * 200 - 100;
      // CAPTURE(objectiveValues);

      // const arma::Mat<double>& variance = arma::randu<arma::Mat<double>>(numberOfParameters, static_cast<arma::uword>(static_cast<int>(numberOfParameters) + (std::bernoulli_distribution(0.5)(mant::Rng::getGenerator()) ? 1 : -1) * std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator()))) * 200 - 100;
      // CAPTURE(variance);

      // CHECK_THROWS_AS(mant::getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, variance), std::logic_error);
    // }

    // SECTION("Throws an exception, if the number of rows of the variance matrix is not equal to the number of parameters.") {
      // const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      // CAPTURE(numberOfDimensions);

      // const arma::uword numberOfParameters = std::uniform_int_distribution<arma::uword>(5, 10)(mant::Rng::getGenerator());
      // CAPTURE(numberOfParameters);
      // const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
      // CAPTURE(parameters);

      // const arma::uword numberOfObjectiveValues = static_cast<arma::uword>(static_cast<int>(numberOfParameters) + (std::bernoulli_distribution(0.5)(mant::Rng::getGenerator()) ? 1 : -1) * std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator()));
      // CAPTURE(numberOfObjectiveValues);
      // const arma::Col<double>& objectiveValues = arma::randu<arma::Col<double>>(numberOfParameters) * 200 - 100;
      // CAPTURE(objectiveValues);

      // const arma::uword numberOfVarianceDimensions = static_cast<arma::uword>(static_cast<int>(numberOfParameters) + (std::bernoulli_distribution(0.5)(mant::Rng::getGenerator()) ? 1 : -1) * std::uniform_int_distribution<arma::uword>(1, 4)(mant::Rng::getGenerator()));
      // CAPTURE(numberOfVarianceDimensions);
      // const arma::Mat<double>& variance = arma::randu<arma::Mat<double>>(numberOfVarianceDimensions, numberOfVarianceDimensions) * 200 - 100;
      // CAPTURE(variance);

      // CHECK_THROWS_AS(mant::getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, variance), std::logic_error);
    // }
  // }
// }
