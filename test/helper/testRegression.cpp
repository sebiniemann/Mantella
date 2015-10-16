// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <cmath>
#include <string>

// Mantella
#include <mantella>

#include <iostream>

TEST_CASE("getOrdinaryLeastSquaresEstimate") {
  SECTION("Returns the ordinary least squares estimates.") {
    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x10.input");
    CAPTURE(parameters);
    
    arma::Row<double> objectiveValues;
    // objectiveValues.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_attractiveSectorFunction_dim40.expected");
    CAPTURE(objectiveValues);
    
    // std::cout << mant::getOrdinaryLeastSquaresEstimate(parameters, objectiveValues) << std::endl;
    // TODO Fix me
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
    
    IS_EQUAL(mant::getOrdinaryLeastSquaresEstimate(parameters, objectiveValues), coefficients);
  }

  SECTION("Exception tests") {
    SECTION("Throws an exception, if the number of parameters is not equal to the number of objective values.") {
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

      CHECK_THROWS_AS(mant::getOrdinaryLeastSquaresEstimate(parameters, objectiveValues), std::logic_error);
    }
  }
}

TEST_CASE("getGeneralisedLeastSquaresEstimate") {
  SECTION("Returns the generalised least squares estimates.") {
    // TODO Fix me
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
    
    const arma::Mat<double>& variance = arma::eye<arma::Mat<double>>(numberOfParameters, numberOfParameters);
    
    IS_EQUAL(mant::getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, variance), coefficients);
  }

  SECTION("Exception tests") {
    SECTION("Throws an exception, if the number of parameters is not equal to the number of objective values.") {
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

      const arma::Mat<double>& variance = getContinuousRandomNumbers(numberOfParameters, numberOfParameters);

      CHECK_THROWS_AS(mant::getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, variance), std::logic_error);
    }

    SECTION("Throws an exception, if the variance matrix is not square.") {
      const arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      const arma::Mat<double>& parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);

      const arma::Row<double>& objectiveValues = getContinuousRandomNumbers(numberOfParameters).t();
      CAPTURE(objectiveValues);

      const arma::Mat<double>& variance = getContinuousRandomNumbers(numberOfParameters, getDifferentDiscreteRandomNumber(numberOfParameters));
      CAPTURE(variance);

      CHECK_THROWS_AS(mant::getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, variance), std::logic_error);
    }
    
    SECTION("Throws an exception, if the number of rows of the variance matrix is not equal to the number of parameters.") {
      const arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      const arma::Mat<double>& parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);

      const arma::Row<double>& objectiveValues = getContinuousRandomNumbers(numberOfParameters).t();
      CAPTURE(objectiveValues);


      const arma::uword numberOfVarianceDimensions = getDifferentDiscreteRandomNumber(numberOfParameters);
      CAPTURE(numberOfVarianceDimensions);
      const arma::Mat<double>& variance = getContinuousRandomNumbers(numberOfVarianceDimensions, numberOfVarianceDimensions);
      CAPTURE(variance);

      CHECK_THROWS_AS(mant::getGeneralisedLeastSquaresEstimate(parameters, objectiveValues, variance), std::logic_error);
    }
  }
}
