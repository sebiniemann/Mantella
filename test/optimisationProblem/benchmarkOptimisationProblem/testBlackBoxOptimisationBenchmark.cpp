// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("bbob::BlackBoxOptimisationBenchmark::BlackBoxOptimisationBenchmark", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark::BlackBoxOptimisationBenchmark]") {
  GIVEN("A number of dimensions") {
    WHEN("The number of dimensions is greater than 0") {
      const arma::uword numberOfDimensions = SYNCHRONISED(getDiscreteRandomNumber());
      CAPTURE(numberOfDimensions);

      THEN("Throw no exception") {
        CHECK_NOTHROW(mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(numberOfDimensions));
      }
    }

    WHEN("The number of dimensions is 0") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(0), std::logic_error);
      }
    }
  }
}

SCENARIO("bbob::BlackBoxOptimisationBenchmark.getLowerBounds", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark.getLowerBounds]") {
  GIVEN("Default lower bounds") {
    const arma::uword numberOfDimensions = SYNCHRONISED(getDiscreteRandomNumber());
    CAPTURE(numberOfDimensions);

    mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(numberOfDimensions);

    THEN("Return the default lower bounds (-5, ..., -5)") {
      IS_EQUAL(optimisationProblem.getLowerBounds(), arma::zeros<arma::Col<double>>(numberOfDimensions) - 5);
    }
  }
}

SCENARIO("bbob::BlackBoxOptimisationBenchmark.getUpperBounds", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark.getUpperBounds]") {
  GIVEN("Default upper bounds") {
    const arma::uword numberOfDimensions = SYNCHRONISED(getDiscreteRandomNumber());
    CAPTURE(numberOfDimensions);

    mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(numberOfDimensions);

    THEN("Return the default upper bounds (5, ..., 5)") {
      IS_EQUAL(optimisationProblem.getUpperBounds(), arma::zeros<arma::Col<double>>(numberOfDimensions) + 5);
    }
  }
}

SCENARIO("bbob::BlackBoxOptimisationBenchmark.getObjectiveValueTranslation", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark.getObjectiveValueTranslation]") {
  GIVEN("Default objective value translation") {
    const arma::uword numberOfDimensions = SYNCHRONISED(getDiscreteRandomNumber());
    CAPTURE(numberOfDimensions);

    mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(numberOfDimensions);

    THEN("Return a randomised objective value translation within [-1000, 1000], based on a Cauchy distribution with location 0.0 and scale 100.0") {
      // TODO
      // Generate 10.000 parameter, initialise the problem 10.000 times and get the translation indirectly from *.getObjectiveValue*.
      // Compare the difference between the parameter and the PDF (Cauchy), assuming that the error should fulfil *IS_UNIFORM*.
    }
  }
}

SCENARIO("bbob::BlackBoxOptimisationBenchmark.getOptimalObjectiveValue", "[bbob::BlackBoxOptimisationBenchmark][bbob::BlackBoxOptimisationBenchmark.getOptimalObjectiveValue]") {
  GIVEN("Default optimal objective value") {
    const arma::uword numberOfDimensions = SYNCHRONISED(getDiscreteRandomNumber());
    CAPTURE(numberOfDimensions);

    mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(numberOfDimensions);

    THEN("Return the same as the objective value translation") {
      CHECK(optimisationProblem.getOptimalObjectiveValue() == Approx(optimisationProblem.getObjectiveValueTranslation()));
    }
  }
}
