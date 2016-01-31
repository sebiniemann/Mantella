// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <memory>

// Mantella
#include <mantella>

class TestOptimisationAlgorithm : public mant::OptimisationAlgorithm {
 public:
  using mant::OptimisationAlgorithm::OptimisationAlgorithm;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::OptimisationAlgorithm::boundariesHandlingFunction_;
  using mant::OptimisationAlgorithm::isStagnatingFunction_;
  using mant::OptimisationAlgorithm::restartingFunction_;
};

SCENARIO("OptimisationAlgorithm::OptimisationAlgorithm", "[OptimisationAlgorithm][OptimisationAlgorithm::OptimisationAlgorithm]") {
  THEN("Throw no exception") {
    CHECK_NOTHROW(mant::OptimisationAlgorithm optimisationAlgorithm);
  }
}

SCENARIO("OptimisationAlgorithm.optimise", "[OptimisationAlgorithm][OptimisationAlgorithm.optimise]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(100);

  GIVEN("An optimisation problem and initial parameters") {
    const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfDimensions);

    mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    WHEN("The next parameters function is not callable") {
      const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(initialParameters);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.optimise(optimisationProblem, initialParameters), std::logic_error);
      }
    }

    WHEN("The next parameters function is callable") {
      optimisationAlgorithm.setNextParametersFunction(
          [&optimisationAlgorithm](
              const arma::uword numberOfDimensions_,
              const arma::Mat<double>& parameters_,
              const arma::Row<double>& objectiveValues_,
              const arma::Row<double>& differences_) {
            return parameters_;
          });

      AND_WHEN("The default boundaries handling function is unchanged") {
        TestOptimisationAlgorithm defaultOptimisationAlgorithm;

        const arma::Mat<double>& parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(parameters);

        THEN("Use the default function") {
          arma::Mat<double> boundedParameter = parameters;
          boundedParameter.elem(arma::find(parameters < 0.0)).fill(0.0);
          boundedParameter.elem(arma::find(parameters > 1.0)).fill(1.0);
          IS_EQUAL(defaultOptimisationAlgorithm.boundariesHandlingFunction_(parameters, parameters<0.0, parameters> 1.0), boundedParameter);
        }
      }

      AND_WHEN("The default is stagnating function is unchanged") {
        TestOptimisationAlgorithm defaultOptimisationAlgorithm;

        const arma::Mat<double>& parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(parameters);

        const arma::Row<double>& objectiveValues = continuousRandomNumbers(numberOfParameters).t();
        CAPTURE(objectiveValues);

        // Skip .optimise()-internal assertion
        const arma::Row<double>& objectiveValuesDifference = arma::zeros<arma::Row<double>>(numberOfParameters) + arma::datum::inf;
        CAPTURE(objectiveValuesDifference);

        THEN("Use the default function") {
          for (arma::uword n = 0; n < 1000; ++n) {
            CHECK(defaultOptimisationAlgorithm.isStagnatingFunction_(parameters, objectiveValues, objectiveValuesDifference) == false);
          }
        }
      }

      AND_WHEN("The default restarting function is unchanged") {
        TestOptimisationAlgorithm defaultOptimisationAlgorithm;

        const arma::Mat<double>& parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(parameters);

        const arma::Row<double>& objectiveValues = continuousRandomNumbers(numberOfParameters).t();
        CAPTURE(objectiveValues);

        // Skip .optimise()-internal assertion
        const arma::Row<double>& objectiveValuesDifference = arma::zeros<arma::Row<double>>(numberOfParameters) + arma::datum::inf;
        CAPTURE(objectiveValuesDifference);

        THEN("Use the default function") {
          arma::Mat<double> restartingParameter(numberOfDimensions, 10000 * numberOfParameters);
          for (arma::uword n = 0; n < 10000; ++n) {
            restartingParameter.cols(n * numberOfParameters, (n + 1) * numberOfParameters - 1) = defaultOptimisationAlgorithm.restartingFunction_(numberOfDimensions, parameters, objectiveValues, objectiveValuesDifference);
          }
          for (arma::uword n = 0; n < restartingParameter.n_rows; ++n) {
            CAPTURE(restartingParameter.row(n));
            IS_UNIFORM(restartingParameter.row(n).t(), 0, 1);
          }
        }
      }

      THEN("Reset all counters (number of iterations and duration), the best objective value and parameter as well as the recorded sampling at the start") {
        const arma::Col<double>& initialParameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(initialParameter);

        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem, initialParameter);
        ::mant::isRecordingSampling = false;

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 100);
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 100);
        CHECK(optimisationAlgorithm.getBestParameter().n_elem == numberOfDimensions);

        const double bestObjectiveValue = optimisationAlgorithm.getBestObjectiveValue();

        const arma::uword differentNumberOfDimensions = SYNCHRONISED(differentDiscreteRandomNumber(numberOfDimensions));
        CAPTURE(differentNumberOfDimensions);

        mant::OptimisationProblem secondOptimisationProblem(differentNumberOfDimensions);
        secondOptimisationProblem.setObjectiveFunction(
            [&secondOptimisationProblem](
                const arma::Col<double>& parameter_) {
              return 100 * arma::norm(parameter_);
            });

        const arma::Mat<double>& secondInitialParameters = continuousRandomNumbers(differentNumberOfDimensions, numberOfParameters);
        CAPTURE(secondInitialParameters);

        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(secondOptimisationProblem, secondInitialParameters);
        ::mant::isRecordingSampling = false;

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 100);
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 100 * numberOfParameters);
        CHECK(optimisationAlgorithm.getBestObjectiveValue() > bestObjectiveValue);
        CHECK(optimisationAlgorithm.getBestParameter().n_elem == differentNumberOfDimensions);
      }

      AND_WHEN("The optimisation problem has no specific objective function and the next parameter function is not changed") {
        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(initialParameters);

        THEN("Throw no exception") {
          CHECK_NOTHROW(optimisationAlgorithm.optimise(optimisationProblem, initialParameters));
        }
      }

      AND_WHEN("At least one initial parameter has an infinite value") {
        arma::Mat<double> initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        initialParameters(0) = arma::datum::inf;
        CAPTURE(initialParameters);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationAlgorithm.optimise(optimisationProblem, initialParameters), std::logic_error);
        }
      }

      AND_WHEN("At least one initial parameter has more or less elements than there are dimensions to optimise") {
        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions - 1, numberOfParameters);
        CAPTURE(initialParameters);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationAlgorithm.optimise(optimisationProblem, initialParameters), std::logic_error);
        }
      }

      AND_WHEN("The initial parameters have zero columns (are empty)") {
        const arma::Mat<double>& initialParameters = arma::Mat<double>(numberOfDimensions, 0);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationAlgorithm.optimise(optimisationProblem, initialParameters), std::logic_error);
        }
      }

      AND_WHEN("At least one lower bound is greater than its upper bound") {
        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);

        optimisationProblem.setLowerBounds(optimisationProblem.getUpperBounds() + 1.0);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationAlgorithm.optimise(optimisationProblem, initialParameters), std::logic_error);
        }
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.setNextParametersFunction", "[OptimisationAlgorithm][OptimisationAlgorithm.setNextParametersFunction]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(100);

  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());

  CAPTURE(numberOfDimensions);

  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  GIVEN("A next parameters function and its name") {
    const std::string& nextParametersFunctionName = "My custom next parameters function name";
    CAPTURE(nextParametersFunctionName);

    WHEN("The next parameters function is callable") {
      auto nextParametersFunction = [&optimisationAlgorithm](
          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setNextParametersFunction(nextParametersFunction, nextParametersFunctionName));
      }

      THEN("Reset all counters (number of (distinct) evaluations), the recorded sampling, the best parameter and objective value as well as why it finished") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(initialParameters);

        optimisationAlgorithm.setNextParametersFunction(nextParametersFunction);
        // Records and increments the counter
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
        ::mant::isRecordingSampling = false;

        optimisationAlgorithm.setNextParametersFunction(nextParametersFunction, nextParametersFunctionName);

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
        CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
        CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
        CHECK(optimisationAlgorithm.getBestParameter().is_empty());
        CHECK(optimisationAlgorithm.isFinished() == false);
        CHECK(optimisationAlgorithm.isTerminated() == false);
      }
    }

    WHEN("The next parameters function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setNextParametersFunction(nullptr, nextParametersFunctionName), std::logic_error);
      }
    }
  }

  GIVEN("A next parameters function") {
    WHEN("The next parameters function is callable") {
      auto nextParametersFunction = [&optimisationAlgorithm](
          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setNextParametersFunction(nextParametersFunction));
      }

      THEN("Reset all counters (number of (distinct) evaluations), the recorded sampling, the best parameter and objective value as well as why it finished") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(initialParameters);

        optimisationAlgorithm.setNextParametersFunction(nextParametersFunction);
        // Records and increments the counter
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
        ::mant::isRecordingSampling = false;

        optimisationAlgorithm.setNextParametersFunction(nextParametersFunction);

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
        CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
        CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
        CHECK(optimisationAlgorithm.getBestParameter().is_empty());
        CHECK(optimisationAlgorithm.isFinished() == false);
        CHECK(optimisationAlgorithm.isTerminated() == false);
      }
    }

    WHEN("The next parameters function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setNextParametersFunction(nullptr), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getNextParametersFunctionName", "[OptimisationAlgorithm][OptimisationAlgorithm.getNextParametersFunctionName]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The default next parameter function is unchanged") {
    THEN("Return an empty string") {
      CHECK(optimisationAlgorithm.getNextParametersFunctionName() == "");
    }
  }

  WHEN("The default next parameter function was changed") {
    auto nextParametersFunction = [&optimisationAlgorithm](
        const arma::uword numberOfDimensions_,
        const arma::Mat<double>& parameters_,
        const arma::Row<double>& objectiveValues_,
        const arma::Row<double>& differences_) {
      return continuousRandomNumbers(numberOfDimensions_);
    };

    AND_WHEN("A new next parameter function name was set") {
      const std::string& nextParametersFunctionName = "My custom next parameter function name";
      CAPTURE(nextParametersFunctionName);

      optimisationAlgorithm.setNextParametersFunction(nextParametersFunction, nextParametersFunctionName);

      THEN("Return the next parameter function name") {
        CHECK(optimisationAlgorithm.getNextParametersFunctionName() == nextParametersFunctionName);
      }
    }

    AND_WHEN("No new next parameter function name was set") {
      optimisationAlgorithm.setNextParametersFunction(nextParametersFunction);

      THEN("Return the default, unnamed next parameter function name") {
        CHECK(optimisationAlgorithm.getNextParametersFunctionName() == "Unnamed, custom next parameter function");
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.setBoundariesHandlingFunction", "[OptimisationAlgorithm][OptimisationAlgorithm.setBoundariesHandlingFunction]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(100);
  optimisationAlgorithm.setNextParametersFunction(
      [&optimisationAlgorithm](
          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      });

  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  GIVEN("A boundaries handling function and its name") {
    const std::string& boundariesHandlingFunctionName = "My custom boundaries handling function name";
    CAPTURE(boundariesHandlingFunctionName);

    WHEN("The boundaries handling function is callable") {
      auto boundariesHandlingFunction = [&optimisationAlgorithm](
          const arma::Mat<double>& parameters_,
          const arma::Mat<arma::uword>& isBelowLowerBound_,
          const arma::Mat<arma::uword>& isAboveUpperBound_) {
        return parameters_;
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setBoundariesHandlingFunction(boundariesHandlingFunction, boundariesHandlingFunctionName));
      }

      THEN("Reset all counters (number of (distinct) evaluations), the recorded sampling, the best parameter and objective value as well as why it finished") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(initialParameters);

        // Records and increments the counter
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
        ::mant::isRecordingSampling = false;

        optimisationAlgorithm.setBoundariesHandlingFunction(boundariesHandlingFunction, boundariesHandlingFunctionName);

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
        CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
        CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
        CHECK(optimisationAlgorithm.getBestParameter().is_empty());
        CHECK(optimisationAlgorithm.isFinished() == false);
        CHECK(optimisationAlgorithm.isTerminated() == false);
      }
    }

    WHEN("The boundaries handling function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setBoundariesHandlingFunction(nullptr, boundariesHandlingFunctionName), std::logic_error);
      }
    }
  }

  GIVEN("A boundaries handling function") {
    WHEN("The boundaries handling function is callable") {
      auto boundariesHandlingFunction = [&optimisationAlgorithm](
          const arma::Mat<double>& parameters_,
          const arma::Mat<arma::uword>& isBelowLowerBound_,
          const arma::Mat<arma::uword>& isAboveUpperBound_) {
        return parameters_;
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setBoundariesHandlingFunction(boundariesHandlingFunction));
      }

      THEN("Reset all counters (number of (distinct) evaluations), the recorded sampling, the best parameter and objective value as well as why it finished") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(initialParameters);

        // Records and increments the counter
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
        ::mant::isRecordingSampling = false;

        optimisationAlgorithm.setBoundariesHandlingFunction(boundariesHandlingFunction);

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
        CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
        CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
        CHECK(optimisationAlgorithm.getBestParameter().is_empty());
        CHECK(optimisationAlgorithm.isFinished() == false);
        CHECK(optimisationAlgorithm.isTerminated() == false);
      }
    }

    WHEN("The boundaries handling function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setBoundariesHandlingFunction(nullptr), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getBoundariesHandlingFunctionName", "[OptimisationAlgorithm][OptimisationAlgorithm.getBoundariesHandlingFunctionName]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The default boundaries handling function is unchanged") {
    THEN("Return 'Map to bound'") {
      CHECK(optimisationAlgorithm.getBoundariesHandlingFunctionName() == "Map to bound");
    }
  }

  WHEN("The default is stagnating function was changed") {
    auto boundariesHandlingFunction = [&optimisationAlgorithm](
        const arma::Mat<double>& parameters_,
        const arma::Mat<arma::uword>& isBelowLowerBound_,
        const arma::Mat<arma::uword>& isAboveUpperBound_) {
      return parameters_;
    };

    AND_WHEN("A new is stagnating function name was set") {
      const std::string& boundariesHandlingFunctionName = "My custom boundaries handling function name";
      CAPTURE(boundariesHandlingFunctionName);

      optimisationAlgorithm.setBoundariesHandlingFunction(boundariesHandlingFunction, boundariesHandlingFunctionName);

      THEN("Return the is stagnating function name") {
        CHECK(optimisationAlgorithm.getBoundariesHandlingFunctionName() == boundariesHandlingFunctionName);
      }
    }

    AND_WHEN("No new is stagnating function name was set") {
      optimisationAlgorithm.setBoundariesHandlingFunction(boundariesHandlingFunction);

      THEN("Return the default, unnamed is stagnating function name") {
        CHECK(optimisationAlgorithm.getBoundariesHandlingFunctionName() == "Unnamed, custom boundaries handling function");
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.setIsStagnatingFunction", "[OptimisationAlgorithm][OptimisationAlgorithm.setIsStagnatingFunction]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(100);
  optimisationAlgorithm.setNextParametersFunction(
      [&optimisationAlgorithm](
          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      });

  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  GIVEN("An is stagnating function and its name") {
    const std::string& isStagnatingFunctionName = "My custom is stagnating function name";
    CAPTURE(isStagnatingFunctionName);

    WHEN("The boundaries handling function is callable") {
      auto isStagnatingFunction = [&optimisationAlgorithm](
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return false;
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setIsStagnatingFunction(isStagnatingFunction, isStagnatingFunctionName));
      }

      THEN("Reset all counters (number of (distinct) evaluations), the recorded sampling, the best parameter and objective value as well as why it finished") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(initialParameters);

        // Records and increments the counter
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
        ::mant::isRecordingSampling = false;

        optimisationAlgorithm.setIsStagnatingFunction(isStagnatingFunction, isStagnatingFunctionName);

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
        CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
        CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
        CHECK(optimisationAlgorithm.getBestParameter().is_empty());
        CHECK(optimisationAlgorithm.isFinished() == false);
        CHECK(optimisationAlgorithm.isTerminated() == false);
      }
    }

    WHEN("The is stagnating function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setIsStagnatingFunction(nullptr, isStagnatingFunctionName), std::logic_error);
      }
    }
  }

  GIVEN("An is stagnating function") {
    WHEN("The is stagnating function is callable") {
      auto isStagnatingFunction = [&optimisationAlgorithm](
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return false;
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setIsStagnatingFunction(isStagnatingFunction));
      }

      THEN("Reset all counters (number of (distinct) evaluations), the recorded sampling, the best parameter and objective value as well as why it finished") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(initialParameters);

        // Records and increments the counter
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
        ::mant::isRecordingSampling = false;

        optimisationAlgorithm.setIsStagnatingFunction(isStagnatingFunction);

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
        CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
        CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
        CHECK(optimisationAlgorithm.getBestParameter().is_empty());
        CHECK(optimisationAlgorithm.isFinished() == false);
        CHECK(optimisationAlgorithm.isTerminated() == false);
      }
    }

    WHEN("The is stagnating function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setIsStagnatingFunction(nullptr), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getIsStagnatingFunctionName", "[OptimisationAlgorithm][OptimisationAlgorithm.getIsStagnatingFunctionName]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The default is stagnating function is unchanged") {
    THEN("Return 'Always false'") {
      CHECK(optimisationAlgorithm.getIsStagnatingFunctionName() == "Always false");
    }
  }

  WHEN("The default is stagnating function was changed") {
    auto isStagnatingFunction = [&optimisationAlgorithm](
        const arma::Mat<double>& parameters_,
        const arma::Row<double>& objectiveValues_,
        const arma::Row<double>& differences_) {
      return false;
    };

    AND_WHEN("A new is stagnating function name was set") {
      const std::string& isStagnatingFunctionName = "My custom is stagnating function name";
      CAPTURE(isStagnatingFunctionName);

      optimisationAlgorithm.setIsStagnatingFunction(isStagnatingFunction, isStagnatingFunctionName);

      THEN("Return the is stagnating function name") {
        CHECK(optimisationAlgorithm.getIsStagnatingFunctionName() == isStagnatingFunctionName);
      }
    }

    AND_WHEN("No new is stagnating function name was set") {
      optimisationAlgorithm.setIsStagnatingFunction(isStagnatingFunction);

      THEN("Return the default, unnamed is stagnating function name") {
        CHECK(optimisationAlgorithm.getIsStagnatingFunctionName() == "Unnamed, custom is stagnation function");
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.setRestartingFunction", "[OptimisationAlgorithm][OptimisationAlgorithm.setRestartingFunction]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(100);
  optimisationAlgorithm.setNextParametersFunction(
      [&optimisationAlgorithm](
          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      });

  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  GIVEN("An restarting function and its name") {
    const std::string& restartingFunctionName = "My custom restarting function name";
    CAPTURE(restartingFunctionName);

    WHEN("The boundaries handling function is callable") {
      auto restartingFunction = [&optimisationAlgorithm](
          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setRestartingFunction(restartingFunction, restartingFunctionName));
      }

      THEN("Reset all counters (number of (distinct) evaluations), the recorded sampling, the best parameter and objective value as well as why it finished") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(initialParameters);

        // Records and increments the counter
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
        ::mant::isRecordingSampling = false;

        optimisationAlgorithm.setRestartingFunction(restartingFunction, restartingFunctionName);

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
        CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
        CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
        CHECK(optimisationAlgorithm.getBestParameter().is_empty());
        CHECK(optimisationAlgorithm.isFinished() == false);
        CHECK(optimisationAlgorithm.isTerminated() == false);
      }
    }

    WHEN("The restarting function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setRestartingFunction(nullptr, restartingFunctionName), std::logic_error);
      }
    }
  }

  GIVEN("An restarting function") {
    WHEN("The restarting function is callable") {
      auto restartingFunction = [&optimisationAlgorithm](
          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationAlgorithm.setRestartingFunction(restartingFunction));
      }

      THEN("Reset all counters (number of (distinct) evaluations), the recorded sampling, the best parameter and objective value as well as why it finished") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        CAPTURE(initialParameters);

        // Records and increments the counter
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
        ::mant::isRecordingSampling = false;

        optimisationAlgorithm.setRestartingFunction(restartingFunction);

        CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
        CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
        CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
        CHECK(optimisationAlgorithm.getBestParameter().is_empty());
        CHECK(optimisationAlgorithm.isFinished() == false);
        CHECK(optimisationAlgorithm.isTerminated() == false);
      }
    }

    WHEN("The restarting function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationAlgorithm.setRestartingFunction(nullptr), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getRestartingFunctionName", "[OptimisationAlgorithm][OptimisationAlgorithm.getRestartingFunctionName]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The default restarting function is unchanged") {
    THEN("Return 'Random'") {
      CHECK(optimisationAlgorithm.getRestartingFunctionName() == "Random");
    }
  }

  WHEN("The default restarting function was changed") {
    auto restartingFunction = [&optimisationAlgorithm](
        const arma::uword numberOfDimensions_,
        const arma::Mat<double>& parameters_,
        const arma::Row<double>& objectiveValues_,
        const arma::Row<double>& differences_) {
      return continuousRandomNumbers(numberOfDimensions_);
    };

    AND_WHEN("A new restarting function name was set") {
      const std::string& restartingFunctionName = "My custom restarting function name";
      CAPTURE(restartingFunctionName);

      optimisationAlgorithm.setRestartingFunction(restartingFunction, restartingFunctionName);

      THEN("Return the restarting function name") {
        CHECK(optimisationAlgorithm.getRestartingFunctionName() == restartingFunctionName);
      }
    }

    AND_WHEN("No new restarting function name was set") {
      optimisationAlgorithm.setRestartingFunction(restartingFunction);

      THEN("Return the default, unnamed restarting function name") {
        CHECK(optimisationAlgorithm.getRestartingFunctionName() == "Unnamed, custom restarting function");
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.setAcceptableObjectiveValue", "[OptimisationAlgorithm][OptimisationAlgorithm.setAcceptableObjectiveValue]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(100);
  optimisationAlgorithm.setNextParametersFunction(
      [&optimisationAlgorithm](
          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      });

  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  GIVEN("An acceptable objective value") {
    const double acceptableObjectiveValue = continuousRandomNumber();
    CAPTURE(acceptableObjectiveValue);

    THEN("Throw no exception") {
      CHECK_NOTHROW(optimisationAlgorithm.setAcceptableObjectiveValue(acceptableObjectiveValue));
    }

    THEN("Do not reset the counters (number of (distinct) evaluations), the recorded sampling, the best parameter or objective value as well as why it finished") {
      const arma::uword numberOfParameters = discreteRandomNumber();
      CAPTURE(numberOfParameters);

      const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(initialParameters);

      // Records and increments the counter
      ::mant::isRecordingSampling = true;
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
      ::mant::isRecordingSampling = false;

      optimisationAlgorithm.setAcceptableObjectiveValue(acceptableObjectiveValue);

      CHECK(optimisationAlgorithm.getNumberOfIterations() > 0);
      CHECK(optimisationAlgorithm.getDuration() > std::chrono::microseconds(0));
      CHECK(optimisationAlgorithm.getRecordedSampling().size() > 0);
      CHECK(optimisationAlgorithm.getBestObjectiveValue() < arma::datum::inf);
      CHECK(!optimisationAlgorithm.getBestParameter().is_empty());
      CHECK((optimisationAlgorithm.isFinished() || optimisationAlgorithm.isTerminated()));
    }
  }
}

SCENARIO("OptimisationAlgorithm.getAcceptableObjectiveValue", "[OptimisationAlgorithm][OptimisationAlgorithm.getAcceptableObjectiveValue]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The default acceptable objective value is unchanged") {
    THEN("Return negative infinity") {
      CHECK(optimisationAlgorithm.getAcceptableObjectiveValue() <= -arma::datum::inf);
    }
  }

  WHEN("The default acceptable objective value was changed") {
    const double acceptableObjectiveValue = continuousRandomNumber();
    CAPTURE(acceptableObjectiveValue);

    optimisationAlgorithm.setAcceptableObjectiveValue(acceptableObjectiveValue);

    THEN("Return the updated acceptable objective value") {
      CHECK(optimisationAlgorithm.getAcceptableObjectiveValue() == Approx(acceptableObjectiveValue));
    }
  }
}

SCENARIO("OptimisationAlgorithm.setMaximalNumberOfIterations", "[OptimisationAlgorithm][OptimisationAlgorithm.setMaximalNumberOfIterations]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(100);
  optimisationAlgorithm.setNextParametersFunction(
      [&optimisationAlgorithm](
          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      });

  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  GIVEN("A maximal number of iterations") {
    const arma::uword maximalNumberOfIterations = discreteRandomNumber();
    CAPTURE(maximalNumberOfIterations);

    THEN("Throw no exception") {
      CHECK_NOTHROW(optimisationAlgorithm.setMaximalNumberOfIterations(maximalNumberOfIterations));
    }

    THEN("Do not reset the counters (number of (distinct) evaluations), the recorded sampling, the best parameter or objective value as well as why it finished") {
      const arma::uword numberOfParameters = discreteRandomNumber();
      CAPTURE(numberOfParameters);

      const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(initialParameters);

      // Records and increments the counter
      ::mant::isRecordingSampling = true;
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
      ::mant::isRecordingSampling = false;

      optimisationAlgorithm.setMaximalNumberOfIterations(maximalNumberOfIterations);

      CHECK(optimisationAlgorithm.getNumberOfIterations() > 0);
      CHECK(optimisationAlgorithm.getDuration() > std::chrono::microseconds(0));
      CHECK(optimisationAlgorithm.getRecordedSampling().size() > 0);
      CHECK(optimisationAlgorithm.getBestObjectiveValue() < arma::datum::inf);
      CHECK(!optimisationAlgorithm.getBestParameter().is_empty());
      CHECK((optimisationAlgorithm.isFinished() || optimisationAlgorithm.isTerminated()));
    }
  }
}

SCENARIO("OptimisationAlgorithm.getMaximalNumberOfIterations", "[OptimisationAlgorithm][OptimisationAlgorithm.getMaximalNumberOfIterations]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The default maximal number of iterations is unchanged") {
    THEN("Return the largest representable, discrete number") {
      CHECK(optimisationAlgorithm.getMaximalNumberOfIterations() == std::numeric_limits<arma::uword>::max());
    }
  }

  WHEN("The default maximal number of iterations was changed") {
    const arma::uword maximalNumberOfIterations = discreteRandomNumber();
    CAPTURE(maximalNumberOfIterations);

    optimisationAlgorithm.setMaximalNumberOfIterations(maximalNumberOfIterations);

    THEN("Return the updated maximal number of iterations") {
      CHECK(optimisationAlgorithm.getMaximalNumberOfIterations() == maximalNumberOfIterations);
    }
  }
}

SCENARIO("OptimisationAlgorithm.setMaximalDuration", "[OptimisationAlgorithm][OptimisationAlgorithm.setMaximalDuration]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(100);
  optimisationAlgorithm.setNextParametersFunction(
      [&optimisationAlgorithm](

          const arma::uword numberOfDimensions_,
          const arma::Mat<double>& parameters_,
          const arma::Row<double>& objectiveValues_,
          const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
      });

  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  GIVEN("A maximal duration") {
    const std::chrono::microseconds maximalDuration = std::chrono::seconds(discreteRandomNumber());
    CAPTURE(maximalDuration.count());

    THEN("Throw no exception") {
      CHECK_NOTHROW(optimisationAlgorithm.setMaximalDuration(maximalDuration));
    }

    THEN("Do not reset the counters (number of (distinct) evaluations), the recorded sampling, the best parameter or objective value as well as why it finished") {
      const arma::uword numberOfParameters = discreteRandomNumber();
      CAPTURE(numberOfParameters);

      const arma::Mat<double>& initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(initialParameters);

      // Records and increments the counter
      ::mant::isRecordingSampling = true;
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
      ::mant::isRecordingSampling = false;

      optimisationAlgorithm.setMaximalDuration(maximalDuration);

      CHECK(optimisationAlgorithm.getNumberOfIterations() > 0);
      CHECK(optimisationAlgorithm.getDuration() > std::chrono::microseconds(0));
      CHECK(optimisationAlgorithm.getRecordedSampling().size() > 0);
      CHECK(optimisationAlgorithm.getBestObjectiveValue() < arma::datum::inf);
      CHECK(!optimisationAlgorithm.getBestParameter().is_empty());
      CHECK((optimisationAlgorithm.isFinished() || optimisationAlgorithm.isTerminated()));
    }
  }
}

SCENARIO("OptimisationAlgorithm.getMaximalDuration", "[OptimisationAlgorithm][OptimisationAlgorithm.getMaximalDuration]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The default maximal duration is unchanged") {
    THEN("Return 10 seconds") {
      CHECK(optimisationAlgorithm.getMaximalDuration() == std::chrono::seconds(10));
    }
  }

  WHEN("The default maximal duration was changed") {
    const std::chrono::microseconds maximalDuration = std::chrono::seconds(discreteRandomNumber());
    CAPTURE(maximalDuration.count());

    optimisationAlgorithm.setMaximalDuration(maximalDuration);

    THEN("Return the updated maximal duration") {
      CHECK(optimisationAlgorithm.getMaximalDuration() == maximalDuration);
    }
  }
}

SCENARIO("OptimisationAlgorithm.isFinished", "[OptimisationAlgorithm][OptimisationAlgorithm.isFinished]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Return false") {
      CHECK(!optimisationAlgorithm.isTerminated());
    }
  }

  WHEN("The optimisation algorithm was started") {
    optimisationAlgorithm.setMaximalNumberOfIterations(100);
    optimisationAlgorithm.setNextParametersFunction(
        [&optimisationAlgorithm](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          return arma::Mat<double>(arma::floor(parameters_ * 10 - 1) / 10);
        });

    const arma::uword numberOfDimensions = 2;
    CAPTURE(numberOfDimensions);

    mant::OptimisationProblem optimisationProblem(numberOfDimensions);
    optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 5);
    optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 5);
    optimisationProblem.setObjectiveFunction(
        [optimisationProblem](
            const arma::Col<double>& parameter) {
          return arma::accu(parameter);
        });

    const arma::uword numberOfParameters = 1 + discreteRandomNumber();
    CAPTURE(numberOfParameters);

    const arma::Mat<double>& initialParameters = arma::ones<arma::Mat<double>>(numberOfDimensions, numberOfParameters);
    CAPTURE(initialParameters);

    WHEN("Found an acceptable parameter in time (its objective value is lower than or equal to the acceptable objective value)") {
      optimisationAlgorithm.setAcceptableObjectiveValue(-1.2);
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return true") {
        CHECK(optimisationAlgorithm.isFinished());
      }
    }

    WHEN("Did not found an acceptable parameter in time") {
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return false") {
        CHECK(!optimisationAlgorithm.isFinished());
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.isTerminated", "[OptimisationAlgorithm][OptimisationAlgorithm.isTerminated]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Return false") {
      CHECK(!optimisationAlgorithm.isTerminated());
    }
  }

  WHEN("The optimisation algorithm was started") {
    optimisationAlgorithm.setMaximalNumberOfIterations(100);
    optimisationAlgorithm.setNextParametersFunction(
        [&optimisationAlgorithm](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          return arma::Mat<double>(arma::floor(parameters_ * 10 - 1) / 10);
        });

    const arma::uword numberOfDimensions = 2;
    CAPTURE(numberOfDimensions);

    mant::OptimisationProblem optimisationProblem(numberOfDimensions);
    optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 5);
    optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 5);
    optimisationProblem.setObjectiveFunction(
        [optimisationProblem](
            const arma::Col<double>& parameter) {
          return arma::accu(parameter);
        });

    const arma::uword numberOfParameters = 1 + discreteRandomNumber();
    CAPTURE(numberOfParameters);

    const arma::Mat<double>& initialParameters = arma::ones<arma::Mat<double>>(numberOfDimensions, numberOfParameters);
    CAPTURE(initialParameters);

    WHEN("Found an acceptable parameter in time (its objective value is lower than or equal to the acceptable objective value)") {
      optimisationAlgorithm.setAcceptableObjectiveValue(-1.2);
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return false") {
        CHECK(!optimisationAlgorithm.isTerminated());
      }
    }

    WHEN("Did not found an acceptable parameter in time") {
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return true") {
        CHECK(optimisationAlgorithm.isTerminated());
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getNumberOfIterations", "[OptimisationAlgorithm][OptimisationAlgorithm.getNumberOfIterations]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Return 0") {
      CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
    }
  }

  WHEN("The optimisation algorithm was started") {
    optimisationAlgorithm.setMaximalNumberOfIterations(100);
    optimisationAlgorithm.setNextParametersFunction(
        [&optimisationAlgorithm](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          return arma::Mat<double>(arma::floor(parameters_ * 10 - 1) / 10);
        });

    const arma::uword numberOfDimensions = 2;
    CAPTURE(numberOfDimensions);

    mant::OptimisationProblem optimisationProblem(numberOfDimensions);
    optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 5);
    optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 5);
    optimisationProblem.setObjectiveFunction(
        [optimisationProblem](
            const arma::Col<double>& parameter) {
          return arma::accu(parameter);
        });

    const arma::uword numberOfParameters = 1 + discreteRandomNumber();
    CAPTURE(numberOfParameters);

    const arma::Mat<double>& initialParameters = arma::ones<arma::Mat<double>>(numberOfDimensions, numberOfParameters);
    CAPTURE(initialParameters);

    WHEN("Found an acceptable parameter in time (its objective value is lower than or equal to the acceptable objective value)") {
      optimisationAlgorithm.setAcceptableObjectiveValue(-1.2);
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return the number of iterations") {
        CHECK(optimisationAlgorithm.getNumberOfIterations() == 7);
      }
    }

    WHEN("Did not found an acceptable parameter in time") {
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return the same as the maximal number of iterations") {
        CHECK(optimisationAlgorithm.getNumberOfIterations() == optimisationAlgorithm.getMaximalNumberOfIterations());
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getDuration", "[OptimisationAlgorithm][OptimisationAlgorithm.getDuration]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Return 0") {
      CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
    }
  }

  WHEN("The optimisation algorithm was started") {
    optimisationAlgorithm.setMaximalDuration(std::chrono::seconds(1));
    optimisationAlgorithm.setNextParametersFunction(
        [&optimisationAlgorithm](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          return arma::Mat<double>(arma::floor(parameters_ * 10 - 1) / 10);
        });

    const arma::uword numberOfDimensions = 2;
    CAPTURE(numberOfDimensions);

    mant::OptimisationProblem optimisationProblem(numberOfDimensions);
    optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 5);
    optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 5);
    optimisationProblem.setObjectiveFunction(
        [optimisationProblem](
            const arma::Col<double>& parameter) {
          return arma::accu(parameter);
        });

    const arma::uword numberOfParameters = 1 + discreteRandomNumber();
    CAPTURE(numberOfParameters);

    const arma::Mat<double>& initialParameters = arma::ones<arma::Mat<double>>(numberOfDimensions, numberOfParameters);
    CAPTURE(initialParameters);

    WHEN("Found an acceptable parameter in time (its objective value is lower than or equal to the acceptable objective value)") {
      optimisationAlgorithm.setAcceptableObjectiveValue(-1.2);
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return the duration") {
        CHECK(optimisationAlgorithm.getDuration() > std::chrono::microseconds(0));
        CHECK(optimisationAlgorithm.getDuration() < optimisationAlgorithm.getMaximalDuration());
      }
    }

    WHEN("Did not found an acceptable parameter in time") {
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return at least the maximal duration (being at most 0.1 second off)") {
        CHECK(optimisationAlgorithm.getDuration() >= optimisationAlgorithm.getMaximalDuration());
        CHECK(optimisationAlgorithm.getDuration() <= optimisationAlgorithm.getMaximalDuration() + std::chrono::milliseconds(100));
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getBestObjectiveValue", "[OptimisationAlgorithm][OptimisationAlgorithm.getBestObjectiveValue]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Return infinity") {
      CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
    }
  }

  WHEN("The optimisation algorithm was started") {
    optimisationAlgorithm.setMaximalNumberOfIterations(100);
    optimisationAlgorithm.setNextParametersFunction(
        [&optimisationAlgorithm](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          return arma::Mat<double>(arma::floor(parameters_ * 10 - 1) / 10);
        });

    const arma::uword numberOfDimensions = 2;
    CAPTURE(numberOfDimensions);

    mant::OptimisationProblem optimisationProblem(numberOfDimensions);
    optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 5);
    optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 5);
    optimisationProblem.setObjectiveFunction(
        [optimisationProblem](
            const arma::Col<double>& parameter) {
          return arma::accu(parameter);
        });

    const arma::uword numberOfParameters = 1 + discreteRandomNumber();
    CAPTURE(numberOfParameters);

    const arma::Mat<double>& initialParameters = arma::ones<arma::Mat<double>>(numberOfDimensions, numberOfParameters);
    CAPTURE(initialParameters);

    WHEN("Found an acceptable parameter in time (its objective value is lower than or equal to the acceptable objective value)") {
      optimisationAlgorithm.setAcceptableObjectiveValue(-1.2);
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return the best objective value") {
        // Due to rounding errors, the 11th parameter will slightly differ from (0.1, ..., 0.1), needing one addition step to reach the optimal solution.
        CHECK(optimisationAlgorithm.getBestObjectiveValue() == Approx(-2.0));
      }
    }

    WHEN("Did not found an acceptable parameter in time") {
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return the best objective value") {
        CHECK(optimisationAlgorithm.getBestObjectiveValue() == Approx(-10.0));
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getBestParameter", "[OptimisationAlgorithm][OptimisationAlgorithm.getBestParameter]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Is empty") {
      CHECK(optimisationAlgorithm.getBestParameter().is_empty());
    }
  }

  WHEN("The optimisation algorithm was started") {
    optimisationAlgorithm.setMaximalNumberOfIterations(100);
    optimisationAlgorithm.setNextParametersFunction(
        [&optimisationAlgorithm](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          return arma::Mat<double>(arma::floor(parameters_ * 10 - 1) / 10);
        });

    const arma::uword numberOfDimensions = 2;
    CAPTURE(numberOfDimensions);

    mant::OptimisationProblem optimisationProblem(numberOfDimensions);
    optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 5);
    optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 5);
    optimisationProblem.setObjectiveFunction(
        [optimisationProblem](
            const arma::Col<double>& parameter) {
          return arma::accu(parameter);
        });

    const arma::uword numberOfParameters = 1 + discreteRandomNumber();
    CAPTURE(numberOfParameters);

    const arma::Mat<double>& initialParameters = arma::ones<arma::Mat<double>>(numberOfDimensions, numberOfParameters);
    CAPTURE(initialParameters);

    WHEN("Found an acceptable parameter in time (its objective value is lower than or equal to the acceptable objective value)") {
      optimisationAlgorithm.setAcceptableObjectiveValue(-1.2);
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return the best parameter") {
        IS_EQUAL(optimisationAlgorithm.getBestParameter(), arma::zeros<arma::Col<double>>(numberOfDimensions) - 1.0);
      }
    }

    WHEN("Did not found an acceptable parameter in time") {
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);

      THEN("Return the best parameter") {
        IS_EQUAL(optimisationAlgorithm.getBestParameter(), arma::zeros<arma::Col<double>>(numberOfDimensions) - 5.0);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getRecordedSampling", "[OptimisationAlgorithm][OptimisationAlgorithm.getRecordedSampling]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Is empty") {
      CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
    }
  }

  WHEN("The optimisation algorithm was started") {
    optimisationAlgorithm.setMaximalNumberOfIterations(13);
    optimisationAlgorithm.setNextParametersFunction(
        [&optimisationAlgorithm](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          return arma::Mat<double>(arma::floor(parameters_ * 10 - 1) / 10);
        });

    const arma::uword numberOfDimensions = 2;
    CAPTURE(numberOfDimensions);

    mant::OptimisationProblem optimisationProblem(numberOfDimensions);
    optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 5);
    optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 5);
    optimisationProblem.setObjectiveFunction(
        [optimisationProblem](
            const arma::Col<double>& parameter) {
          return arma::accu(parameter);
        });

    const arma::uword numberOfParameters = 1 + discreteRandomNumber();
    CAPTURE(numberOfParameters);

    const arma::Mat<double>& initialParameters = arma::ones<arma::Mat<double>>(numberOfDimensions, numberOfParameters);
    CAPTURE(initialParameters);

    AND_WHEN("Sampling recording was disabled") {
      ::mant::isRecordingSampling = false;
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
      ::mant::isRecordingSampling = false;

      THEN("Return an empty set") {
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
      }
    }

    AND_WHEN("Sampling recording was enabled") {
      std::vector<std::pair<arma::Col<double>, double>> expectedSampling;
      for (arma::uword n = 0; n < optimisationAlgorithm.getMaximalNumberOfIterations(); ++n) {
        const double stepSize = std::max(0.0, 1.0 - 0.1 * static_cast<double>(n));
        for (arma::uword k = 0; k < numberOfParameters; ++k) {
          expectedSampling.push_back({arma::zeros<arma::Col<double>>(numberOfDimensions) + stepSize, stepSize * 2});
        }
      }

      ::mant::isRecordingSampling = true;
      optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
      ::mant::isRecordingSampling = false;

      THEN("Return the recorded sampling") {
        IS_EQUAL(optimisationAlgorithm.getRecordedSampling(), expectedSampling);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.reset", "[OptimisationAlgorithm][OptimisationAlgorithm.reset]") {
  mant::OptimisationAlgorithm optimisationAlgorithm;

  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  WHEN("The optimisation problem was processed") {
    optimisationAlgorithm.setNextParametersFunction(
        [&optimisationAlgorithm](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
        },
        "My custom next parameters function name");
    optimisationAlgorithm.setBoundariesHandlingFunction(
        [&optimisationAlgorithm](
            const arma::Mat<double>& parameters_,
            const arma::Mat<arma::uword>& isBelowLowerBound_,
            const arma::Mat<arma::uword>& isAboveUpperBound_) {
        return parameters_;
        },
        "My custom boundaries handling function name");
    optimisationAlgorithm.setIsStagnatingFunction(
        [&optimisationAlgorithm](
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
        return false;
        },
        "My custom is stagnating function name");
    optimisationAlgorithm.setRestartingFunction(
        [&optimisationAlgorithm](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
        return continuousRandomNumbers(numberOfDimensions_);
        },
        "My custom restarting function name");

    const double acceptableObjectiveValue = continuousRandomNumber();
    optimisationAlgorithm.setAcceptableObjectiveValue(acceptableObjectiveValue);
    CAPTURE(acceptableObjectiveValue);

    const arma::uword maximalNumberOfIterations = discreteRandomNumber();
    optimisationAlgorithm.setMaximalNumberOfIterations(maximalNumberOfIterations);
    CAPTURE(maximalNumberOfIterations);

    const std::chrono::microseconds maximalDuration = std::chrono::milliseconds(discreteRandomNumber());
    optimisationAlgorithm.setMaximalDuration(maximalDuration);
    CAPTURE(maximalDuration.count());

    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    arma::Mat<double> initialParameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
    CAPTURE(initialParameters);

    ::mant::isRecordingSampling = true;
    optimisationAlgorithm.optimise(optimisationProblem, initialParameters);
    ::mant::isRecordingSampling = false;

    THEN("Reset only the counters (number of (distinct) evaluations), the recorded sampling, the best parameter or objective value as well as why it finished") {
      optimisationAlgorithm.reset();

      CHECK(optimisationAlgorithm.getNextParametersFunctionName() == "My custom next parameters function name");
      CHECK(optimisationAlgorithm.getBoundariesHandlingFunctionName() == "My custom boundaries handling function name");
      CHECK(optimisationAlgorithm.getIsStagnatingFunctionName() == "My custom is stagnating function name");
      CHECK(optimisationAlgorithm.getRestartingFunctionName() == "My custom restarting function name");
      CHECK(optimisationAlgorithm.getAcceptableObjectiveValue() == Approx(acceptableObjectiveValue));
      CHECK(optimisationAlgorithm.getMaximalNumberOfIterations() == maximalNumberOfIterations);
      CHECK(optimisationAlgorithm.getMaximalDuration() == maximalDuration);

      CHECK(optimisationAlgorithm.getNumberOfIterations() == 0);
      CHECK(optimisationAlgorithm.getDuration() == std::chrono::microseconds(0));
      CHECK(optimisationAlgorithm.getRecordedSampling().size() == 0);
      CHECK(optimisationAlgorithm.getBestObjectiveValue() >= arma::datum::inf);
      CHECK(optimisationAlgorithm.getBestParameter().is_empty());
      CHECK(optimisationAlgorithm.isFinished() == false);
      CHECK(optimisationAlgorithm.isTerminated() == false);
    }
  }
}
