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
      samples.insert({continuousRandomNumbers(parameters.n_rows + 1), continuousRandomNumber()});

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::fitnessDistanceCorrelation(samples), std::logic_error);
      }
    }

    WHEN("The number of samples is less than 2") {
      samples.clear();
      samples.insert({continuousRandomNumbers(parameters.n_rows), continuousRandomNumber()});

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
      samples.insert({continuousRandomNumbers(parameters.n_rows + 1), continuousRandomNumber()});

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::lipschitzContinuity(samples), std::logic_error);
      }
    }

    WHEN("The number of samples is less than 2") {
      samples.clear();
      samples.insert({continuousRandomNumbers(parameters.n_rows), continuousRandomNumber()});

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::lipschitzContinuity(samples), std::logic_error);
      }
    }
  }
}

SCENARIO("additiveSeparability", "[samplesAnalysis][additiveSeparability]") {
  GIVEN("An optimisation problem, a number of evaluations, a maximal deviation and a minimal confidence") {
    const arma::uword numberOfEvaluations = 100;
    CAPTURE(numberOfEvaluations);

    WHEN("The maximal deviation is non-negative and the minimal confidence interval is within (0, 1]") {
      AND_WHEN("The problem is fully separable") {
        const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
        CAPTURE(numberOfDimensions);

        mant::OptimisationProblem optimisationProblem(numberOfDimensions);
        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
              return arma::sum(parameter_);
            });

        const double maximalDeviation = std::abs(continuousRandomNumber());
        CAPTURE(maximalDeviation);

        const double minimalConfidence = std::min(1.0, 1.0 / std::abs(continuousRandomNumber()));
        CAPTURE(minimalConfidence);

        THEN("Return one dimension per part") {
          std::vector<arma::Col<arma::uword>> expected;
          for (arma::uword n = 0; n < numberOfDimensions; ++n) {
            expected.push_back({n});
          }

          IS_EQUAL(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, minimalConfidence), expected);
        }
      }

      AND_WHEN("The problem is partly separable") {
        const arma::uword numberOfDimensions = 7;
        CAPTURE(numberOfDimensions);

        mant::OptimisationProblem optimisationProblem(numberOfDimensions);
        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
              return parameter_(0) * 0.01 * parameter_(1) + parameter_(1) + parameter_(2) * parameter_(3) + parameter_(4) * parameter_(5) * parameter_(6);
            });

        const double maximalDeviation = 0.0;
        CAPTURE(maximalDeviation);

        const double minimalConfidence = 1.0;
        CAPTURE(minimalConfidence);

        THEN("Return the separation") {
          IS_EQUAL(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, minimalConfidence), {{0, 1}, {2, 3}, {4, 5, 6}});
        }

        AND_WHEN("The maximal deviation is large enough to ignore small influence between dimensions") {
          optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 1.0);
          optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 1.0);

          const double greaterMaximalDeviation = maximalDeviation + 2.0;
          CAPTURE(maximalDeviation);

          THEN("Return a finer separation") {
            CHECK(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, greaterMaximalDeviation, minimalConfidence).size() > mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, minimalConfidence).size());
            IS_EQUAL(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, greaterMaximalDeviation, minimalConfidence), {{0}, {1}, {2}, {3}, {4}, {5}, {6}});
          }
        }

        AND_WHEN("The confidence is low enough to ignore small influence between dimensions") {
          optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 1.0);
          optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 1.0);

          const double lesserMinimalConfidence = minimalConfidence - 0.9;
          CAPTURE(minimalConfidence);

          THEN("Return a finer separation") {
            CHECK(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, lesserMinimalConfidence).size() > mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, minimalConfidence).size());
            IS_EQUAL(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, lesserMinimalConfidence), {{0}, {1}, {2}, {3}, {4}, {5}, {6}});
          }
        }
      }

      AND_WHEN("The problem is not separable") {
        const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
        CAPTURE(numberOfDimensions);

        mant::OptimisationProblem optimisationProblem(numberOfDimensions);
        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
              return arma::prod(parameter_);
            });

        const double maximalDeviation = 0.0;
        CAPTURE(maximalDeviation);

        const double minimalConfidence = 1.0;
        CAPTURE(minimalConfidence);

        THEN("Return a single part, containing all dimensions") {
          IS_EQUAL(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, minimalConfidence), {mant::range(0, numberOfDimensions - 1)});
        }
      }
    }

    WHEN("At least one lower bound is greater than its upper bound") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);
      optimisationProblem.setLowerBounds(optimisationProblem.getUpperBounds() + 1.0);

      const double maximalDeviation = std::abs(continuousRandomNumber());
      CAPTURE(maximalDeviation);

      const double minimalConfidence = std::min(1.0, 1.0 / std::abs(continuousRandomNumber()));
      CAPTURE(minimalConfidence);

      CHECK_THROWS_AS(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, minimalConfidence), std::logic_error);
    }

    WHEN("The maximal deviation is negative") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

      const double maximalDeviation = -std::abs(continuousRandomNumber()) - 1.0;
      CAPTURE(maximalDeviation);

      const double minimalConfidence = std::min(1.0, 1.0 / std::abs(continuousRandomNumber()));
      CAPTURE(minimalConfidence);

      CHECK_THROWS_AS(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, minimalConfidence), std::logic_error);
    }

    WHEN("The minimal confidence is less than or equal to 0") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

      const double maximalDeviation = std::abs(continuousRandomNumber());
      CAPTURE(maximalDeviation);

      const double minimalConfidence = -std::abs(continuousRandomNumber());
      CAPTURE(minimalConfidence);

      CHECK_THROWS_AS(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, minimalConfidence), std::logic_error);
    }

    WHEN("The minimal confidence is greater than 1") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

      const double maximalDeviation = std::abs(continuousRandomNumber());
      CAPTURE(maximalDeviation);

      const double minimalConfidence = std::abs(continuousRandomNumber()) + 2.0;
      CAPTURE(minimalConfidence);

      CHECK_THROWS_AS(mant::additiveSeparability(optimisationProblem, numberOfEvaluations, maximalDeviation, minimalConfidence), std::logic_error);
    }
  }
}
