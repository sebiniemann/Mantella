// Catch
#include <catch.hpp>

// C++ standard library
#include <cstdlib>
#include <cmath>

// Mantella
#include <mantella>

TEST_CASE(
    "getOrdinaryLeastSquaresEstimate") {
  SECTION(
      "Returns the ordinary least squares estimates.") {
  }

  SECTION(
      "Returns the exact coefficients for a linear function.") {
  }

  SECTION(
      "Exception tests") {
    SECTION(
        "Throws an exception, if the number of parameters is not equal to the number of objective values.") {
      const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      CAPTURE(numberOfDimensions);

      const arma::uword numberOfParameters = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      CAPTURE(numberOfParameters);
      const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
      CAPTURE(parameters);

      const arma::uword numberOfObjectiveValues = numberOfParameters + (std::bernoulli_distribution(0.5)(mant::Rng::getGenerator()) ? 1.0 : -1.0) * std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      CAPTURE(numberOfObjectiveValues);
      const arma::Col<double>& objectiveValues = arma::randu<arma::Col<double>>(numberOfParameters) * 200 - 100;
      CAPTURE(objectiveValues);

      CHECK_THROWS_AS(mant::getOrdinaryLeastSquaresEstimate(parameters, objectiveValues), std::logic_error);
    }
  }
}

TEST_CASE(
    "getGeneralisedLeastSquaresEstimate") {
  SECTION(
      "Returns the generalised least squares estimates.") {
  }

  SECTION(
      "Returns the exact coefficients for a linear function.") {
  }

  SECTION(
      "Exception tests") {
    SECTION(
        "Throws an exception, if the number of parameters is not equal to the number of objective values.") {
      const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      CAPTURE(numberOfDimensions);

      const arma::uword numberOfParameters = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      CAPTURE(numberOfParameters);
      const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
      CAPTURE(parameters);

      const arma::uword numberOfObjectiveValues = numberOfParameters + (std::bernoulli_distribution(0.5)(mant::Rng::getGenerator()) ? 1.0 : -1.0) * std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      CAPTURE(numberOfObjectiveValues);
      const arma::Col<double>& objectiveValues = arma::randu<arma::Col<double>>(numberOfParameters) * 200 - 100;
      CAPTURE(objectiveValues);

      const arma::Mat<double>& variance = arma::randu<arma::Mat<double>>(numberOfParameters, numberOfParameters) * 200 - 100;
      CAPTURE(variance);

      CHECK_THROWS_AS(mant::getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, variance), std::logic_error);
    }

    SECTION(
        "Throws an exception, if the variance matrix is not square.") {
    }

    SECTION(
        "Throws an exception, if the number of rows of the variance matrix is not equal to the number of parameters.") {
    }
  }
}