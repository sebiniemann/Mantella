// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <cstdlib>
#include <cmath>

// Mantella
#include <mantella>

TEST_CASE("medianAbsoluteError") {
  SECTION("Returns the median absolute error.") {
    const arma::Row<double>& randomValues = getContinuousRandomNumbers(1000).t();
    CAPTURE(arma::sort(randomValues, "descend"));
  
    CHECK(mant::medianAbsoluteError(randomValues) == Approx(arma::median(arma::abs(randomValues - arma::median(randomValues)))));
  }
}

TEST_CASE("percentile") {
  const arma::Row<double>& randomValues = getContinuousRandomNumbers(1000).t();
  CAPTURE(arma::sort(randomValues, "descend"));
    
  SECTION("Returns the percentile.") {
    const arma::Row<double>& sortedValues = arma::sort(randomValues, "descend");
    CAPTURE(sortedValues);
    
    CHECK(mant::percentile(randomValues, 1.0) == Approx(0.01 * sortedValues(9) + 0.99 * sortedValues(10)));
  }

  SECTION("Exception tests:") {
    SECTION("Throws an exception, if the percentile is lesser than or equal to 0.") {
      CHECK_THROWS_AS(mant::percentile(randomValues, 0), std::logic_error);
    }
    
    SECTION("Throws an exception, if the percentile is greater than 100.") {
      CHECK_THROWS_AS(mant::percentile(randomValues, 100.1), std::logic_error);
    }
  }
}

TEST_CASE("decile") {
  const arma::Row<double>& randomValues = getContinuousRandomNumbers(1000).t();
  CAPTURE(arma::sort(randomValues, "descend"));
    
  SECTION("Returns the decile.") {
    CHECK(mant::decile(randomValues, 9.0) == Approx(mant::percentile(randomValues, 90.0)));
  }

  SECTION("Exception tests:") {
    SECTION("Throws an exception, if the percentile is lesser than or equal to 0.") {
      CHECK_THROWS_AS(mant::decile(randomValues, 0), std::logic_error);
    }
    
    SECTION("Throws an exception, if the percentile is greater than 10.") {
      CHECK_THROWS_AS(mant::decile(randomValues, 10.1), std::logic_error);
    }
  }
}

TEST_CASE("quartile") {
    const arma::Row<double>& randomValues = getContinuousRandomNumbers(1000).t();
    CAPTURE(arma::sort(randomValues, "descend"));
  SECTION("Returns the quartile.") {
    CHECK(mant::quartile(randomValues, 3.0) == Approx(mant::percentile(randomValues, 75.0)));
  }
  
  SECTION("Exception tests:") {
    SECTION("Throws an exception, if the percentile is lesser than or equal to 0.") {
      CHECK_THROWS_AS(mant::quartile(randomValues, 0), std::logic_error);
    }
    
    SECTION("Throws an exception, if the percentile is greater than 4.") {
      CHECK_THROWS_AS(mant::quartile(randomValues, 4.1), std::logic_error);
    }
  }
}

TEST_CASE("ordinaryLeastSquaresEstimate") {
  SECTION("Returns the ordinary least squares estimates with sufficient precision.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);

    const arma::uword numberOfParameters = 10000;
    CAPTURE(numberOfParameters);
    
    arma::Mat<double> parameters(numberOfDimensions + 1, numberOfParameters);
    parameters.head_rows(numberOfDimensions) = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
    parameters.row(numberOfDimensions).ones();

    const arma::Col<double>& coefficients = getContinuousRandomNumbers(numberOfDimensions + 1);
    CAPTURE(coefficients);
    
    arma::Row<double> objectiveValues(numberOfParameters);
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      objectiveValues(n) = arma::dot(parameters.col(n), coefficients);
    }
    objectiveValues += arma::randn<arma::Row<double>>(numberOfParameters);
    CAPTURE(objectiveValues);
    
    IS_EQUAL(static_cast<arma::Col<double>>(arma::floor(mant::ordinaryLeastSquaresEstimate(parameters, objectiveValues))), static_cast<arma::Col<double>>(arma::floor(coefficients)));
  }

  SECTION("Returns the exact coefficients for a linear function.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);

    const arma::uword numberOfParameters = numberOfDimensions + getDiscreteRandomNumber();
    CAPTURE(numberOfParameters);
    
    arma::Mat<double> parameters(numberOfDimensions + 1, numberOfParameters);
    parameters.head_rows(numberOfDimensions) = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
    parameters.row(numberOfDimensions).ones();

    const arma::Col<double>& coefficients = getContinuousRandomNumbers(numberOfDimensions + 1);
    CAPTURE(coefficients);
    
    arma::Row<double> objectiveValues(numberOfParameters);
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      objectiveValues(n) = arma::dot(parameters.col(n), coefficients);
    }
    CAPTURE(objectiveValues);
    
    IS_EQUAL(mant::ordinaryLeastSquaresEstimate(parameters, objectiveValues), coefficients);
  }

  SECTION("Exception tests:") {
    SECTION("Throws an exception, if the number of parameters is unequal to the number of objective values.") {
      const arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      const arma::Mat<double>& parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);

      const arma::uword numberOfObjectiveValues = getDifferentDiscreteRandomNumber(numberOfParameters);
      CAPTURE(numberOfObjectiveValues);
      const arma::Row<double>& objectiveValues = getContinuousRandomNumbers(numberOfObjectiveValues).t();
      CAPTURE(objectiveValues);

      CHECK_THROWS_AS(mant::ordinaryLeastSquaresEstimate(parameters, objectiveValues), std::logic_error);
    }
  }
}

TEST_CASE("generalisedLeastSquaresEstimate") {
  SECTION("Returns the generalised least squares estimates.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);

    const arma::uword numberOfParameters = numberOfDimensions + getDiscreteRandomNumber();
    CAPTURE(numberOfParameters);
    
    arma::Mat<double> parameters(numberOfDimensions + 1, numberOfParameters);
    parameters.head_rows(numberOfDimensions) = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
    parameters.row(numberOfDimensions).ones();

    const arma::Col<double>& coefficients = getContinuousRandomNumbers(numberOfDimensions + 1);
    CAPTURE(coefficients);
    
    arma::Row<double> objectiveValues(numberOfParameters);
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      objectiveValues(n) = arma::dot(parameters.col(n), coefficients);
    }
    CAPTURE(objectiveValues);
    
    const arma::Mat<double>& covariance = arma::eye<arma::Mat<double>>(numberOfParameters, numberOfParameters);
    
    IS_EQUAL(static_cast<arma::Col<double>>(arma::floor(mant::generalisedLeastSquaresEstimate(parameters, objectiveValues, covariance))), static_cast<arma::Col<double>>(arma::floor(coefficients)));
  }

  SECTION("Returns the same value as an ordinary least squares estimates with an identity matrix as covariance matrix.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);

    const arma::uword numberOfParameters = numberOfDimensions + getDiscreteRandomNumber();
    CAPTURE(numberOfParameters);
    
    arma::Mat<double> parameters(numberOfDimensions + 1, numberOfParameters);
    parameters.head_rows(numberOfDimensions) = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
    parameters.row(numberOfDimensions).ones();

    const arma::Col<double>& coefficients = getContinuousRandomNumbers(numberOfDimensions + 1);
    CAPTURE(coefficients);
    
    arma::Row<double> objectiveValues(numberOfParameters);
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      objectiveValues(n) = arma::dot(parameters.col(n), coefficients);
    }
    CAPTURE(objectiveValues);
    
    const arma::Mat<double>& covariance = arma::eye<arma::Mat<double>>(numberOfParameters, numberOfParameters);
    
    IS_EQUAL(mant::generalisedLeastSquaresEstimate(parameters, objectiveValues, covariance), mant::ordinaryLeastSquaresEstimate(parameters, objectiveValues));
  }

  SECTION("Returns the exact coefficients for a linear function.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);

    const arma::uword numberOfParameters = numberOfDimensions + getDiscreteRandomNumber();
    CAPTURE(numberOfParameters);
    
    arma::Mat<double> parameters(numberOfDimensions + 1, numberOfParameters);
    parameters.head_rows(numberOfDimensions) = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
    parameters.row(numberOfDimensions).ones();

    const arma::Col<double>& coefficients = getContinuousRandomNumbers(numberOfDimensions + 1);
    CAPTURE(coefficients);
    
    arma::Row<double> objectiveValues(numberOfParameters);
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      objectiveValues(n) = arma::dot(parameters.col(n), coefficients);
    }
    CAPTURE(objectiveValues);
    
    const arma::Mat<double>& covariance = arma::eye<arma::Mat<double>>(numberOfParameters, numberOfParameters);
    
    IS_EQUAL(mant::generalisedLeastSquaresEstimate(parameters, objectiveValues, covariance), coefficients);
  }

  SECTION("Exception tests:") {
    SECTION("Throws an exception, if the number of parameters is unequal to the number of objective values.") {
      const arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      const arma::Mat<double>& parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);

      const arma::uword numberOfObjectiveValues = getDifferentDiscreteRandomNumber(numberOfParameters);
      CAPTURE(numberOfObjectiveValues);
      const arma::Row<double>& objectiveValues = getContinuousRandomNumbers(numberOfObjectiveValues).t();
      CAPTURE(objectiveValues);

      const arma::Mat<double>& covariance = getContinuousRandomNumbers(numberOfParameters, numberOfParameters);

      CHECK_THROWS_AS(mant::generalisedLeastSquaresEstimate(parameters, objectiveValues, covariance), std::logic_error);
    }
    
    SECTION("Throws an exception, if the number of rows of the covariance matrix is unequal to the number of parameters.") {
      const arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      const arma::Mat<double>& parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);

      const arma::Row<double>& objectiveValues = getContinuousRandomNumbers(numberOfParameters).t();
      CAPTURE(objectiveValues);


      const arma::uword numberOfcovarianceDimensions = getDifferentDiscreteRandomNumber(numberOfParameters);
      CAPTURE(numberOfcovarianceDimensions);
      const arma::Mat<double>& covariance = getContinuousRandomNumbers(numberOfcovarianceDimensions, numberOfcovarianceDimensions);
      CAPTURE(covariance);

      CHECK_THROWS_AS(mant::generalisedLeastSquaresEstimate(parameters, objectiveValues, covariance), std::logic_error);
    }

    SECTION("Throws an exception, if the covariance matrix is not positive semi-definite.") {
      const arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      const arma::Mat<double>& parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);

      const arma::Row<double>& objectiveValues = getContinuousRandomNumbers(numberOfParameters).t();
      CAPTURE(objectiveValues);

      const arma::Mat<double>& covariance = -arma::eye(numberOfParameters, numberOfParameters);
      CAPTURE(covariance);

      CHECK_THROWS_AS(mant::generalisedLeastSquaresEstimate(parameters, objectiveValues, covariance), std::logic_error);
    }
  }
}
