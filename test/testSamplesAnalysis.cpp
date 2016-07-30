// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

extern std::string testDirectory;

SCENARIO("fitnessDistanceCorrelation", "[samplesAnalysis][fitnessDistanceCorrelation]") {
  GIVEN("A map") {
    WHEN("The map's standard deviation is 0") {
      THEN("Return NaN") {
        CHECK(std::isnan(mant::fitnessDistanceCorrelation(std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}, {{2.0}, -1.0}}))) == true);
      }
    }

    WHEN("The map contains more than 1 elements having a non-zero standard deviation") {
      THEN("Return the fitness distance correlation") {
        CHECK(mant::fitnessDistanceCorrelation(std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0, -3.0}, -1.0}, {{1.0, -4.0}, 0.0}, {{2.0, -5.0}, 2.0}, {{3.0, -5.5}, 5.0}})) == Approx(0.9814270509));
      }
    }
  }
}

SCENARIO("lipschitzContinuity", "[samplesAnalysis][lipschitzContinuity]") {
  GIVEN("A map") {
    WHEN("The map contains more than 1 elements having a non-zero standard deviation") {
      THEN("Return the Lipschitz constant") {
        CHECK(mant::lipschitzContinuity(std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0, -3.0}, -1.0}, {{2.0, -3.0}, 4.0}, {{2.0, -5.0}, 2.0}, {{3.0, -5.5}, 5.0}})) == Approx(2.5));
      }
    }
  }
}

SCENARIO("additiveSeparability", "[samplesAnalysis][additiveSeparability]") {
  GIVEN("An optimisation problem, a number of evaluations and a minimal confidence") {
    WHEN("The minimal confidence is 0") {
      THEN("Return {{0}, {1}, ..., {n}}") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions(
            {{[](
                  const arma::vec& parameter_) {
                return arma::accu(parameter_);
              },
              "Test function"}});

        CHECK(hasSameElements(mant::additiveSeparability(optimisationProblem, 100, 0.0), {{0}, {1}}) == true);
      }
    }

    WHEN("The optimisation problem is not separable") {
      THEN("Return {{0, 1, ..., n}}") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions(
            {{[](
                  const arma::vec& parameter_) {
                return arma::prod(parameter_);
              },
              "Test function"}});

        CHECK(hasSameElements(mant::additiveSeparability(optimisationProblem, 100, 0.9), {{0, 1}}) == true);
      }
    }

    WHEN("The optimisation problem is partly separable") {
      THEN("Return the partly separation") {
        mant::OptimisationProblem optimisationProblem(3);
        optimisationProblem.setObjectiveFunctions(
            {{[](
                  const arma::vec& parameter_) {
                return parameter_(0) * parameter_(2) + parameter_(1);
              },
              "Test function"}});

        CHECK(hasSameElements(mant::additiveSeparability(optimisationProblem, 100, 0.9), {{0, 2}, {1}}) == true);
      }
    }

    WHEN("The number of evaluations is fully separable") {
      THEN("Return {{0}, {1}, ..., {n}}") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions(
            {{[](
                  const arma::vec& parameter_) {
                return arma::accu(arma::square(parameter_));
              },
              "Test function"}});

        CHECK(hasSameElements(mant::additiveSeparability(optimisationProblem, 100, 0.9), {{0}, {1}}) == true);
      }
    }
  }
}
