// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("OptimisationProblem.numberOfDimensions_", "[OptimisationProblem][OptimisationProblem.numberOfDimensions_]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());

  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  THEN("Return the number of dimensions_") {
    CHECK(optimisationProblem.numberOfDimensions_ == numberOfDimensions);
  }
}

SCENARIO("OptimisationProblem::OptimisationProblem", "[OptimisationProblem][OptimisationProblem::OptimisationProblem]") {
  GIVEN("A number of dimensions") {
    WHEN("The number of dimensions is greater than 0") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      THEN("Throw no exception") {
        CHECK_NOTHROW(mant::OptimisationProblem optimisationProblem(numberOfDimensions));
      }
    }

    WHEN("The number of dimensions is 0") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::OptimisationProblem optimisationProblem(0), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.setObjectiveFunction", "[OptimisationProblem][OptimisationProblem.setObjectiveFunction]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  GIVEN("An objective function and its name") {
    const std::string& objectiveFunctionnName = "My custom objective function name";
    CAPTURE(objectiveFunctionnName);

    WHEN("The objective function is callable") {
      auto objectiveFunction = [&optimisationProblem](
          const arma::Col<double>& parameter_) {
        return 0;
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setObjectiveFunction(objectiveFunction, objectiveFunctionnName));
      }

      THEN("Reset all counters (number of (distinct) evaluations) and cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(objectiveFunction);
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setObjectiveFunction(objectiveFunction, objectiveFunctionnName);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }
    }

    WHEN("The objective function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setObjectiveFunction(nullptr, objectiveFunctionnName), std::logic_error);
      }
    }
  }

  GIVEN("An objective function") {
    WHEN("The objective function is callable") {
      auto objectiveFunction = [&optimisationProblem](
          const arma::Col<double>& parameter_) {
        return 0;
      };

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setObjectiveFunction(objectiveFunction));
      }

      THEN("Reset all counters (number of (distinct) evaluations) and cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(objectiveFunction);
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setObjectiveFunction(objectiveFunction);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }
    }

    WHEN("The objective function is not callable") {
      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setObjectiveFunction(nullptr), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getObjectiveFunctionName", "[OptimisationProblem][OptimisationProblem.getObjectiveFunctionName]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The default objective function is unchanged") {
    THEN("Return an empty string") {
      CHECK(optimisationProblem.getObjectiveFunctionName() == "");
    }
  }

  WHEN("The default objective function was changed") {
    auto objectiveFunction = [&optimisationProblem](
        const arma::Col<double>& parameter_) {
      return 0;
    };

    AND_WHEN("A new objective function name was set") {
      const std::string& objectiveFunctionName = "My custom objective function name";
      CAPTURE(objectiveFunctionName);

      optimisationProblem.setObjectiveFunction(objectiveFunction, objectiveFunctionName);

      THEN("Return the objective function name") {
        CHECK(optimisationProblem.getObjectiveFunctionName() == objectiveFunctionName);
      }
    }

    AND_WHEN("No new objective function name was set") {
      optimisationProblem.setObjectiveFunction(objectiveFunction);

      THEN("Return the default, unnamed objective function name") {
        CHECK(optimisationProblem.getObjectiveFunctionName() == "Unnamed, custom optimisation problem");
      }
    }
  }
}

SCENARIO("OptimisationProblem.getObjectiveValue", "[OptimisationProblem][OptimisationProblem.getObjectiveValue]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  GIVEN("A parameter") {
    WHEN("The default objective function is unchanged") {
      const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameter);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
      }
    }

    WHEN("The default objective function was changed") {
      auto objectiveFunction = [&optimisationProblem](
          const arma::Col<double>& parameter_) {
        return arma::accu(parameter_);
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);

      AND_WHEN("All parameters are finite and have exactly [numberOfDimensions] rows") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        arma::Mat<double> parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        // Duplicated all parameters and shuffle them afterwards, to observe if it works as expected with/without caching.
        parameters = arma::shuffle(arma::join_rows(parameters, parameters));
        CAPTURE(parameters);

        AND_WHEN("All parameter and objective value modifications are non-default (including the minimal parameter distance)") {
          const arma::Col<arma::uword>& parameterPermutation = SYNCHRONISED(mant::randomPermutationVector(numberOfDimensions));
          optimisationProblem.setParameterPermutation(parameterPermutation);
          CAPTURE(parameterPermutation);

          const arma::Col<double>& parameterScaling = SYNCHRONISED(continuousRandomNumbers(numberOfDimensions));
          optimisationProblem.setParameterScaling(parameterScaling);
          CAPTURE(parameterScaling);

          const arma::Col<double>& parameterTranslation = SYNCHRONISED(continuousRandomNumbers(numberOfDimensions));
          optimisationProblem.setParameterTranslation(parameterTranslation);
          CAPTURE(parameterTranslation);

          const arma::Mat<double>& parameterRotation = SYNCHRONISED(mant::randomRotationMatrix(numberOfDimensions));
          optimisationProblem.setParameterRotation(parameterRotation);
          CAPTURE(parameterRotation);

          // Objective value space modifiers
          const double objectiveValueScaling = SYNCHRONISED(continuousRandomNumber());
          optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
          CAPTURE(objectiveValueScaling);

          const double objectiveValueTranslation = SYNCHRONISED(continuousRandomNumber());
          optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
          CAPTURE(objectiveValueTranslation);

          THEN("Return the objective value") {
            ::mant::isCachingSamples = true;
            for (arma::uword n = 0; n < parameters.n_cols; ++n) {
              const arma::Col<double>& parameter = parameters.col(n);
              CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveValueScaling * objectiveFunction(parameterRotation * (parameterScaling % parameter.elem(parameterPermutation) - parameterTranslation)) + objectiveValueTranslation));
            }
            ::mant::isCachingSamples = false;
          }

          AND_WHEN("Caching was enabled") {
            THEN("Return the same objective value as when caching is disabled") {
              ::mant::isCachingSamples = false;
              for (arma::uword n = 0; n < parameters.n_cols; ++n) {
                const arma::Col<double>& parameter = parameters.col(n);
                CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveValueScaling * objectiveFunction(parameterRotation * (parameterScaling % parameter.elem(parameterPermutation) - parameterTranslation)) + objectiveValueTranslation));
              }
              ::mant::isCachingSamples = false;
            }
          }
        }
      }

      AND_WHEN("The parameter has less then [numberOfDimensions] elements") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions - 1);
        CAPTURE(parameter);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
        }
      }

      AND_WHEN("The parameter has more then [numberOfDimensions] elements") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions + discreteRandomNumber());
        CAPTURE(parameter);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
        }
      }

      AND_WHEN("The parameter is infinite") {
        arma::Col<double> parameter = continuousRandomNumbers(numberOfDimensions);
        parameter(0) = arma::datum::inf;
        CAPTURE(parameter);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
        }
      }
    }
  }
}

SCENARIO("OptimisationProblem.getNormalisedObjectiveValue", "[OptimisationProblem][OptimisationProblem.getNormalisedObjectiveValue]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  GIVEN("A normalised parameter") {
    WHEN("The default objective function is unchanged") {
      const arma::Col<double>& parameter = arma::normalise(continuousRandomNumbers(numberOfDimensions));
      CAPTURE(parameter);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
      }
    }

    WHEN("The default objective function was changed") {
      auto objectiveFunction = [&optimisationProblem](
          const arma::Col<double>& parameter_) {
        return arma::accu(parameter_);
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);

      AND_WHEN("All parameters are finite and have exactly [numberOfDimensions] rows") {
        const arma::uword numberOfParameters = discreteRandomNumber();
        CAPTURE(numberOfParameters);

        arma::Mat<double> parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
        // Duplicated all parameters and shuffle them afterwards, to observe if it works as expected with/without caching.
        parameters = arma::shuffle(arma::join_rows(parameters, parameters));
        CAPTURE(parameters);

        AND_WHEN("All parameter and objective value modifications are non-default (including the minimal parameter distance)") {
          // Parameter space modifiers
          const arma::Col<arma::uword>& parameterPermutation = SYNCHRONISED(mant::randomPermutationVector(numberOfDimensions));
          optimisationProblem.setParameterPermutation(parameterPermutation);
          CAPTURE(parameterPermutation);

          const arma::Col<double>& parameterScaling = SYNCHRONISED(continuousRandomNumbers(numberOfDimensions));
          optimisationProblem.setParameterScaling(parameterScaling);
          CAPTURE(parameterScaling);

          const arma::Col<double>& parameterTranslation = SYNCHRONISED(continuousRandomNumbers(numberOfDimensions));
          optimisationProblem.setParameterTranslation(parameterTranslation);
          CAPTURE(parameterTranslation);

          const arma::Mat<double>& parameterRotation = SYNCHRONISED(mant::randomRotationMatrix(numberOfDimensions));
          optimisationProblem.setParameterRotation(parameterRotation);

          CAPTURE(parameterRotation);

          // Objective value space modifiers
          const double objectiveValueScaling = SYNCHRONISED(continuousRandomNumber());
          optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
          CAPTURE(objectiveValueScaling);

          const double objectiveValueTranslation = SYNCHRONISED(continuousRandomNumber());
          optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
          CAPTURE(objectiveValueTranslation);

          THEN("Return the objective value") {
            ::mant::isCachingSamples = true;
            for (arma::uword n = 0; n < parameters.n_cols; ++n) {
              const arma::Col<double>& parameter = parameters.col(n);
              CHECK(optimisationProblem.getNormalisedObjectiveValue((parameter - optimisationProblem.getLowerBounds()) / (optimisationProblem.getUpperBounds() - optimisationProblem.getLowerBounds())) == Approx(objectiveValueScaling * objectiveFunction(parameterRotation * (parameterScaling % parameter.elem(parameterPermutation) - parameterTranslation)) + objectiveValueTranslation));
            }
            ::mant::isCachingSamples = false;
          }

          AND_WHEN("Caching was enabled") {
            THEN("Return the same objective values as when caching is disabled") {
              ::mant::isCachingSamples = false;
              for (arma::uword n = 0; n < parameters.n_cols; ++n) {
                const arma::Col<double>& parameter = parameters.col(n);
                CHECK(optimisationProblem.getNormalisedObjectiveValue((parameter - optimisationProblem.getLowerBounds()) / (optimisationProblem.getUpperBounds() - optimisationProblem.getLowerBounds())) == Approx(objectiveValueScaling * objectiveFunction(parameterRotation * (parameterScaling % parameter.elem(parameterPermutation) - parameterTranslation)) + objectiveValueTranslation));
              }
              ::mant::isCachingSamples = false;
            }
          }
        }
      }

      AND_WHEN("The parameter has less then [numberOfDimensions] elements") {
        const arma::Col<double>& parameter = arma::normalise(continuousRandomNumbers(numberOfDimensions - 1));
        CAPTURE(parameter);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
        }
      }

      AND_WHEN("The parameter has more then [numberOfDimensions] elements") {
        const arma::Col<double>& parameter = arma::normalise(continuousRandomNumbers(numberOfDimensions + discreteRandomNumber()));
        CAPTURE(parameter);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
        }
      }

      AND_WHEN("The parameter is infinite") {
        arma::Col<double> parameter = arma::normalise(continuousRandomNumbers(numberOfDimensions));
        parameter(0) = arma::datum::inf;
        CAPTURE(parameter);

        THEN("Throw a std::logic_error") {
          CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
        }
      }
    }
  }
}

SCENARIO("OptimisationProblem.setLowerBounds", "[OptimisationProblem][OptimisationProblem.setLowerBounds]") {
  GIVEN("Some lower bounds") {
    WHEN("The lower bounds are finite and have exactly [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& lowerBounds = continuousRandomNumbers(numberOfDimensions);
      CAPTURE(lowerBounds);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setLowerBounds(lowerBounds));
      }

      THEN("Do not reset the counters (number of (distinct) evaluations) or cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setLowerBounds(lowerBounds);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 1);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 1);
        CHECK(optimisationProblem.getCachedSamples().size() == 1);
      }

#if defined(SUPPORT_MPI)
      THEN("The lower bounds are synchronised over MPI") {
        optimisationProblem.setLowerBounds(lowerBounds);
        IS_EQUAL(optimisationProblem.getLowerBounds(), SYNCHRONISED(lowerBounds));
      }
#endif
    }

    WHEN("The lower bound is greater than the upper bound") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& upperBounds = continuousRandomNumbers(numberOfDimensions);
      CAPTURE(upperBounds);

      optimisationProblem.setUpperBounds(upperBounds);

      const arma::Col<double>& lowerBounds = upperBounds + arma::ones<arma::Col<double>>(numberOfDimensions);
      CAPTURE(lowerBounds);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setLowerBounds(lowerBounds));
      }
    }

    WHEN("The lower bounds have less then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& lowerBounds = continuousRandomNumbers(numberOfDimensions - 1);
      CAPTURE(lowerBounds);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds(lowerBounds), std::logic_error);
      }
    }

    WHEN("The lower bounds have more then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& lowerBounds = continuousRandomNumbers(numberOfDimensions + discreteRandomNumber());
      CAPTURE(lowerBounds);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds(lowerBounds), std::logic_error);
      }
    }

    WHEN("At least one lower bound is infinite") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      arma::Col<double> lowerBounds = continuousRandomNumbers(numberOfDimensions);
      lowerBounds(0) = arma::datum::inf;
      CAPTURE(lowerBounds);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds(lowerBounds), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getLowerBounds", "[OptimisationProblem][OptimisationProblem.getLowerBounds]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  GIVEN("Default lower bounds") {
    THEN("Return the default lower bounds (-10, ..., -10)^n") {
      IS_EQUAL(optimisationProblem.getLowerBounds(), arma::zeros<arma::Col<double>>(numberOfDimensions) - 10);
    }
  }

  GIVEN("An updated lower bounds") {
    const arma::Col<double>& lowerBounds = continuousRandomNumbers(numberOfDimensions);
    CAPTURE(lowerBounds);

    optimisationProblem.setLowerBounds(lowerBounds);

    THEN("Return the updated lower bounds") {
      IS_EQUAL(optimisationProblem.getLowerBounds(), SYNCHRONISED(lowerBounds));
    }
  }
}

SCENARIO("OptimisationProblem.setUpperBounds", "[OptimisationProblem][OptimisationProblem.setUpperBounds]") {
  GIVEN("Some upper bounds") {
    WHEN("The upper bounds are finite and have exactly [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& upperBounds = continuousRandomNumbers(numberOfDimensions);
      CAPTURE(upperBounds);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setUpperBounds(upperBounds));
      }

      THEN("Do not reset the counters (number of (distinct) evaluations) or cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setUpperBounds(upperBounds);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 1);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 1);
        CHECK(optimisationProblem.getCachedSamples().size() == 1);
      }

#if defined(SUPPORT_MPI)
      THEN("The upper bounds are synchronised over MPI") {
        optimisationProblem.setUpperBounds(upperBounds);
        IS_EQUAL(optimisationProblem.getUpperBounds(), SYNCHRONISED(upperBounds));
      }
#endif
    }

    WHEN("The upper bounds are greater than the lower bounds") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& lowerBounds = continuousRandomNumbers(numberOfDimensions);
      CAPTURE(lowerBounds);

      optimisationProblem.setLowerBounds(lowerBounds);

      const arma::Col<double>& upperBounds = lowerBounds - arma::ones<arma::Col<double>>(numberOfDimensions);
      CAPTURE(upperBounds);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setUpperBounds(upperBounds));
      }
    }

    WHEN("The upper bounds have less then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& upperBounds = continuousRandomNumbers(numberOfDimensions - 1);
      CAPTURE(upperBounds);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds(upperBounds), std::logic_error);
      }
    }
  }

  GIVEN("An updated lower bounds") {
    const arma::Col<double>& lowerBounds = getContinuousRandomNumbers(numberOfDimensions);
    CAPTURE(lowerBounds);

    optimisationProblem.setLowerBounds(lowerBounds);

    THEN("Return the updated lower bounds") {
      IS_EQUAL(optimisationProblem.getLowerBounds(), SYNCHRONISED(lowerBounds));
    }
  }
}

SCENARIO("OptimisationProblem.setUpperBounds", "[OptimisationProblem][OptimisationProblem.setUpperBounds]") {
  GIVEN("Some upper bounds") {
    WHEN("The upper bounds are finite and have exactly [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(getDiscreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& upperBounds = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(upperBounds);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setUpperBounds(upperBounds));
      }

      THEN("Do not reset the counters (number of (distinct) evaluations) or cache") {
        const arma::Col<double>& parameter = getContinuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

    WHEN("The upper bounds have more then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& upperBounds = continuousRandomNumbers(numberOfDimensions + discreteRandomNumber());
      CAPTURE(upperBounds);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds(upperBounds), std::logic_error);
      }
    }

    WHEN("At least one upper bound is infinite") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      arma::Col<double> upperBounds = continuousRandomNumbers(numberOfDimensions);
      upperBounds(0) = arma::datum::inf;
      CAPTURE(upperBounds);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds(upperBounds), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getUpperBounds", "[OptimisationProblem][OptimisationProblem.getUpperBounds]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  GIVEN("Default upper bounds") {
    THEN("Return the default upper bounds (10, ..., 10)^n") {
      IS_EQUAL(optimisationProblem.getUpperBounds(), arma::zeros<arma::Col<double>>(numberOfDimensions) + 10);
    }
  }

  GIVEN("An updated upper bounds") {
    const arma::Col<double>& upperBounds = continuousRandomNumbers(numberOfDimensions);
    CAPTURE(upperBounds);

    optimisationProblem.setUpperBounds(upperBounds);

    THEN("Return the updated upper bounds") {
      IS_EQUAL(optimisationProblem.getUpperBounds(), SYNCHRONISED(upperBounds));
    }
  }
}

SCENARIO("OptimisationProblem.setParameterPermutation", "[OptimisationProblem][OptimisationProblem.setParameterPermutation]") {
  GIVEN("A parameter permutation") {
    WHEN("The parameter permutation has [numberOfDimensions] unique elements, all within [0, [numberOfDimensions] - 1]") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<arma::uword> parameterPermutation = mant::randomPermutationVector(numberOfDimensions);
      CAPTURE(parameterPermutation);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setParameterPermutation(parameterPermutation));
      }

      THEN("Reset the counters (number of (distinct) evaluations) and cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setParameterPermutation(parameterPermutation);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }

#if defined(SUPPORT_MPI)
      THEN("The parameter permutation is synchronised over MPI") {
        optimisationProblem.setParameterPermutation(parameterPermutation);
        IS_EQUAL(optimisationProblem.getParameterPermutation(), SYNCHRONISED(parameterPermutation));
      }
#endif
    }

    WHEN("The parameter permutation has less then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<arma::uword>& parameterPermutation = mant::randomPermutationVector(numberOfDimensions - 1);
      CAPTURE(parameterPermutation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutation), std::logic_error);
      }
    }

    WHEN("The parameter permutation has more then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<arma::uword>& parameterPermutation = mant::randomPermutationVector(numberOfDimensions + discreteRandomNumber());
      CAPTURE(parameterPermutation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutation), std::logic_error);
      }
    }

    WHEN("The parameter permutation has at least one element not in [0, [numberOfElements] - 1]") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      arma::Col<arma::uword> parameterPermutation = mant::randomPermutationVector(numberOfDimensions);
      parameterPermutation(0) = numberOfDimensions;
      CAPTURE(parameterPermutation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutation), std::logic_error);
      }
    }

    WHEN("The parameter permutation has non-unique elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      arma::Col<arma::uword> parameterPermutation = mant::randomPermutationVector(numberOfDimensions);
      parameterPermutation(0) = parameterPermutation(1);
      CAPTURE(parameterPermutation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutation), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getParameterPermutation", "[OptimisationProblem][OptimisationProblem.getParameterPermutation]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The default parameter permutation is unchanged") {
    THEN("Return [0, ..., [numberOfDimensions] - 1]") {
      IS_EQUAL(optimisationProblem.getParameterPermutation(), mant::range(0, numberOfDimensions - 1));
    }
  }

  WHEN("The default parameter permutation was changed") {
    const arma::Col<arma::uword>& parameterPermutation = mant::randomPermutationVector(numberOfDimensions);
    CAPTURE(parameterPermutation);

    optimisationProblem.setParameterPermutation(parameterPermutation);

    THEN("Return the updated parameter permutation") {
      IS_EQUAL(optimisationProblem.getParameterPermutation(), SYNCHRONISED(parameterPermutation));
    }
  }
}

SCENARIO("OptimisationProblem.setParameterScaling", "[OptimisationProblem][OptimisationProblem.setParameterScaling]") {
  GIVEN("A parameter scaling") {
    WHEN("The parameter scaling is finite and has exactly [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& parameterScaling = continuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameterScaling);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setParameterScaling(parameterScaling));
      }

      THEN("Reset the counters (number of (distinct) evaluations) and cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setParameterScaling(parameterScaling);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }

#if defined(SUPPORT_MPI)
      THEN("The parameter scaling is synchronised over MPI") {
        optimisationProblem.setParameterScaling(parameterScaling);
        IS_EQUAL(optimisationProblem.getParameterScaling(), SYNCHRONISED(parameterScaling));
      }
#endif
    }

    WHEN("The parameter scaling has less then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& parameterScaling = continuousRandomNumbers(numberOfDimensions - 1);
      CAPTURE(parameterScaling);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(parameterScaling), std::logic_error);
      }
    }

    WHEN("The parameter scaling has more then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& parameterScaling = continuousRandomNumbers(numberOfDimensions + discreteRandomNumber());
      CAPTURE(parameterScaling);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(parameterScaling), std::logic_error);
      }
    }

    WHEN("At least one element in the parameter scaling is infinite") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      arma::Col<double> parameterScaling = continuousRandomNumbers(numberOfDimensions);
      parameterScaling(0) = arma::datum::inf;
      CAPTURE(parameterScaling);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(parameterScaling), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getParameterScaling", "[OptimisationProblem][OptimisationProblem.getParameterScaling]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The default parameter scaling is unchanged") {
    THEN("Return a vector of ones") {
      IS_EQUAL(optimisationProblem.getParameterScaling(), arma::ones<arma::Col<double>>(numberOfDimensions));
    }
  }

  WHEN("The default parameter scaling was changed") {
    const arma::Col<double>& parameterScaling = continuousRandomNumbers(numberOfDimensions);
    CAPTURE(parameterScaling);

    optimisationProblem.setParameterScaling(parameterScaling);

    THEN("Return the updated parameter permutation") {
      IS_EQUAL(optimisationProblem.getParameterScaling(), SYNCHRONISED(parameterScaling));
    }
  }
}

SCENARIO("OptimisationProblem.setParameterTranslation", "[OptimisationProblem][OptimisationProblem.setParameterTranslation]") {
  GIVEN("A parameter translation") {
    WHEN("The parameter translation is finite and has exactly [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& parameterTranslation = continuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameterTranslation);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setParameterTranslation(parameterTranslation));
      }

      THEN("Reset the counters (number of (distinct) evaluations) and cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setParameterTranslation(parameterTranslation);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }

#if defined(SUPPORT_MPI)
      THEN("The parameter translation is synchronised over MPI") {
        optimisationProblem.setParameterTranslation(parameterTranslation);
        IS_EQUAL(optimisationProblem.getParameterTranslation(), SYNCHRONISED(parameterTranslation));
      }
#endif
    }

    WHEN("The parameter translation has less then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& parameterTranslation = continuousRandomNumbers(numberOfDimensions - 1);
      CAPTURE(parameterTranslation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(parameterTranslation), std::logic_error);
      }
    }

    WHEN("The parameter translation has more then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& parameterTranslation = continuousRandomNumbers(numberOfDimensions + discreteRandomNumber());
      CAPTURE(parameterTranslation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(parameterTranslation), std::logic_error);
      }
    }

    WHEN("At least one element in the parameter translation is infinite") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      arma::Col<double> parameterTranslation = continuousRandomNumbers(numberOfDimensions + discreteRandomNumber());
      parameterTranslation(0) = arma::datum::inf;
      CAPTURE(parameterTranslation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(parameterTranslation), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getParameterTranslation", "[OptimisationProblem][OptimisationProblem.getParameterTranslation]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The default parameter translation is unchanged") {
    THEN("Return a vector of zeros") {
      IS_EQUAL(optimisationProblem.getParameterTranslation(), arma::zeros<arma::Col<double>>(numberOfDimensions));
    }
  }

  WHEN("The default parameter translation was changed") {
    const arma::Col<double>& parameterTranslation = continuousRandomNumbers(numberOfDimensions);
    CAPTURE(parameterTranslation);

    optimisationProblem.setParameterTranslation(parameterTranslation);

    THEN("Return the updated parameter translation") {
      IS_EQUAL(optimisationProblem.getParameterTranslation(), SYNCHRONISED(parameterTranslation));
    }
  }
}

SCENARIO("OptimisationProblem.setParameterRotation", "[OptimisationProblem][OptimisationProblem.setParameterRotation]") {
  GIVEN("A parameter rotation") {
    WHEN("The parameter rotation is orthogonal") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Mat<double>& parameterRotation = mant::randomRotationMatrix(numberOfDimensions);
      CAPTURE(parameterRotation);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setParameterRotation(parameterRotation));
      }

      THEN("Reset the counters (number of (distinct) evaluations) and cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setParameterRotation(parameterRotation);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }

#if defined(SUPPORT_MPI)
      THEN("The parameter rotation is synchronised over MPI") {
        optimisationProblem.setParameterRotation(parameterRotation);
        IS_EQUAL(optimisationProblem.getParameterRotation(), SYNCHRONISED(parameterRotation));
      }
#endif
    }

    WHEN("The parameter rotation has less than [numberOfDimensions] rows or columns") {
      const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Mat<double>& parameterRotation = mant::randomRotationMatrix(numberOfDimensions - 1);
      CAPTURE(parameterRotation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
      }
    }

    WHEN("The parameter rotation has more than [numberOfDimensions] rows or columns") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Mat<double>& parameterRotation = mant::randomRotationMatrix(numberOfDimensions + discreteRandomNumber());
      CAPTURE(parameterRotation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
      }
    }

    WHEN("The parameter rotation is not square") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Mat<double>& parameterRotation = continuousRandomNumbers(numberOfDimensions, 1 + numberOfDimensions);
      CAPTURE(parameterRotation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
      }
    }

    WHEN("The parameter rotation's inverse is not equal to its transpose") {
      const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      arma::Mat<double> parameterRotation = mant::randomRotationMatrix(numberOfDimensions);
      // Increasing an element of an orthonormal rotation matrix by 1 should result in an inequality transpose and inverse.
      parameterRotation(0, 0)++;
      CAPTURE(parameterRotation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getParameterRotation", "[OptimisationProblem][OptimisationProblem.getParameterRotation]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The default parameter rotation is unchanged") {
    THEN("Return an identity matrix") {
      IS_EQUAL(optimisationProblem.getParameterRotation(), arma::eye<arma::Mat<double>>(numberOfDimensions, numberOfDimensions));
    }
  }

  WHEN("The default parameter rotation was changed") {
    const arma::Mat<double>& parameterRotation = mant::randomRotationMatrix(numberOfDimensions);
    CAPTURE(parameterRotation);

    optimisationProblem.setParameterRotation(parameterRotation);

    THEN("Return the updated parameter rotation") {
      IS_EQUAL(optimisationProblem.getParameterRotation(), SYNCHRONISED(parameterRotation));
    }
  }
}

SCENARIO("OptimisationProblem.setObjectiveValueScaling", "[OptimisationProblem][OptimisationProblem.setObjectiveValueScaling]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  GIVEN("An objective value scaling") {
    WHEN("The objective value scaling is finite") {
      const double objectiveValueScaling = continuousRandomNumber();
      CAPTURE(objectiveValueScaling);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setObjectiveValueScaling(objectiveValueScaling));
      }

      THEN("Reset the counters (number of (distinct) evaluations) and cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }

#if defined(SUPPORT_MPI)
      THEN("The objective value scaling is synchronised over MPI") {
        optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
        CHECK(optimisationProblem.getObjectiveValueScaling() == Approx(SYNCHRONISED(objectiveValueScaling)));
      }
#endif
    }

    WHEN("The objective value scaling is infinite") {
      const double objectiveValueScaling = arma::datum::inf;
      CAPTURE(objectiveValueScaling);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setObjectiveValueScaling(objectiveValueScaling), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getObjectiveValueScaling", "[OptimisationProblem][OptimisationProblem.getObjectiveValueScaling]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The default objective value scaling is unchanged") {
    THEN("Return 1") {
      CHECK(optimisationProblem.getObjectiveValueScaling() == 1.0);
    }
  }

  WHEN("The default objective value scaling was changed") {
    const double objectiveValueScaling = continuousRandomNumber();
    CAPTURE(objectiveValueScaling);

    optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);

    THEN("Return the updated objective value scaling") {
      CHECK(optimisationProblem.getObjectiveValueScaling() == Approx(SYNCHRONISED(objectiveValueScaling)));
    }
  }
}

SCENARIO("OptimisationProblem.setObjectiveValueTranslation", "[OptimisationProblem][OptimisationProblem.setObjectiveValueTranslation]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  GIVEN("An objective value translation") {
    WHEN("The objective value translation is finite") {
      const double objectiveValueTranslation = continuousRandomNumber();
      CAPTURE(objectiveValueTranslation);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation));
      }

      THEN("Reset the counters (number of (distinct) evaluations) and cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }

#if defined(SUPPORT_MPI)
      THEN("The objective value translation is synchronised over MPI") {
        optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
        CHECK(optimisationProblem.getObjectiveValueTranslation() == Approx(SYNCHRONISED(objectiveValueTranslation)));
      }
#endif
    }

    WHEN("The objective value translation is infinite") {
      const double objectiveValueTranslation = arma::datum::inf;
      CAPTURE(objectiveValueTranslation);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getObjectiveValueTranslation", "[OptimisationProblem][OptimisationProblem.getObjectiveValueTranslation]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The default objective value translation is unchanged") {
    THEN("Return 0") {
      CHECK(optimisationProblem.getObjectiveValueTranslation() == 0.0);
    }
  }

  WHEN("The default objective value translation was changed") {
    const double objectiveValueTranslation = continuousRandomNumber();
    CAPTURE(objectiveValueTranslation);

    optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);

    THEN("Return the updated objective value translation") {
      CHECK(optimisationProblem.getObjectiveValueTranslation() == Approx(SYNCHRONISED(objectiveValueTranslation)));
    }
  }
}

SCENARIO("OptimisationProblem.getCachedSamples", "[OptimisationProblem][OptimisationProblem.getCachedSamples]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Return an empty set") {
      CHECK(optimisationProblem.getCachedSamples().size() == 0);
    }
  }

  WHEN("The optimisation problem was processed") {
    auto objectiveFunction = [](
        const arma::Col<double>& parameter) {
      return arma::accu(parameter % mant::range(1, parameter.n_elem));
    };
    optimisationProblem.setObjectiveFunction(objectiveFunction);

    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    arma::Mat<double> parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
    // Duplicated all parameters and shuffle them afterwards, to observe if it works as expected with/without caching.
    parameters = arma::shuffle(arma::join_rows(parameters, parameters));
    CAPTURE(parameters);

    AND_WHEN("Caching was disabled") {
      ::mant::isCachingSamples = false;

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      ::mant::isCachingSamples = false;

      THEN("Return an empty set") {
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }
    }

    AND_WHEN("Caching was enabled") {
      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> expectedSamples;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double> parameter = parameters.col(n);
        expectedSamples.insert({parameter, objectiveFunction(parameter)});
      }
    }

      ::mant::isCachingSamples = true;

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      ::mant::isCachingSamples = false;

      THEN("Return the cached samples") {
        HAS_SAME_SAMPLES(optimisationProblem.getCachedSamples(), expectedSamples);
      }
    }
  }
}

SCENARIO("OptimisationProblem.setMinimalParameterDistance", "[OptimisationProblem][OptimisationProblem.setMinimalParameterDistance]") {
  GIVEN("A minimal parameter distance") {
    WHEN("The minimal parameter distance is finite, positive (including 0) and has exactly [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& minimalParameterDistance = arma::abs(continuousRandomNumbers(numberOfDimensions));
      CAPTURE(minimalParameterDistance);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setMinimalParameterDistance(minimalParameterDistance));
      }

      THEN("Do not reset the counters (number of (distinct) evaluations) or cache") {
        const arma::Col<double>& parameter = continuousRandomNumbers(numberOfDimensions);
        CAPTURE(parameter);

        optimisationProblem.setObjectiveFunction(
            [&optimisationProblem](
                const arma::Col<double>& parameter_) {
            return arma::accu(parameter_);
            });
        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue(parameter);
        ::mant::isCachingSamples = false;

        optimisationProblem.setMinimalParameterDistance(minimalParameterDistance);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 1);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 1);
        CHECK(optimisationProblem.getCachedSamples().size() == 1);
      }

#if defined(SUPPORT_MPI)
      THEN("The minimal parameter distance is synchronised over MPI") {
        optimisationProblem.setMinimalParameterDistance(minimalParameterDistance);
        IS_EQUAL(optimisationProblem.getMinimalParameterDistance(), SYNCHRONISED(minimalParameterDistance));
      }
#endif
    }

    WHEN("The minimal parameter distance has less then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& minimalParameterDistance = arma::abs(continuousRandomNumbers(numberOfDimensions - 1));
      CAPTURE(minimalParameterDistance);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setMinimalParameterDistance(minimalParameterDistance), std::logic_error);
      }
    }

    WHEN("The minimal parameter distance has more then [numberOfDimensions] elements") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& minimalParameterDistance = arma::abs(continuousRandomNumbers(numberOfDimensions + discreteRandomNumber()));
      CAPTURE(minimalParameterDistance);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setMinimalParameterDistance(minimalParameterDistance), std::logic_error);
      }
    }

    WHEN("At least one element in the minimal parameter distance is negative") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      const arma::Col<double>& minimalParameterDistance = -arma::abs(continuousRandomNumbers(numberOfDimensions));
      CAPTURE(minimalParameterDistance);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setMinimalParameterDistance(minimalParameterDistance), std::logic_error);
      }
    }

    WHEN("At least one element in the minimal parameter distance is infinite") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);

      mant::OptimisationProblem optimisationProblem(numberOfDimensions);

      arma::Col<double> minimalParameterDistance = arma::abs(continuousRandomNumbers(numberOfDimensions));
      minimalParameterDistance(0) = arma::datum::inf;
      CAPTURE(minimalParameterDistance);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setMinimalParameterDistance(minimalParameterDistance), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getMinimalParameterDistance", "[OptimisationProblem][OptimisationProblem.getMinimalParameterDistance]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The default minimal parameter distance is unchanged") {
    THEN("Return a vector of zeros") {
      IS_EQUAL(optimisationProblem.getMinimalParameterDistance(), arma::zeros<arma::Col<double>>(numberOfDimensions));
    }
  }

  WHEN("The default minimal parameter distance was changed") {
    const arma::Col<double>& minimalParameterDistance = arma::abs(continuousRandomNumbers(numberOfDimensions));
    CAPTURE(minimalParameterDistance);

    optimisationProblem.setMinimalParameterDistance(minimalParameterDistance);

    THEN("Return the updated minimal parameter distance") {
      IS_EQUAL(optimisationProblem.getMinimalParameterDistance(), SYNCHRONISED(minimalParameterDistance));
    }
  }
}

SCENARIO("OptimisationProblem.getNumberOfEvaluations", "[OptimisationProblem][OptimisationProblem.getNumberOfEvaluations]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Return 0") {
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
    }
  }

  WHEN("The optimisation problem was processed") {
    auto objectiveFunction = [](
        const arma::Col<double>& parameter) {
      return arma::accu(parameter % mant::range(1, parameter.n_elem));
    };
    optimisationProblem.setObjectiveFunction(objectiveFunction);

    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    arma::Mat<double> parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
    // Duplicated all parameters and shuffle them afterwards, to observe if it works as expected with/without caching.
    parameters = arma::shuffle(arma::join_rows(parameters, parameters));
    CAPTURE(parameters);

    THEN("Return the number of evaluations, whether caching was enabled or not") {
      ::mant::isCachingSamples = true;

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      ::mant::isCachingSamples = false;

      CHECK(optimisationProblem.getNumberOfEvaluations() == 2 * numberOfParameters);
    }
  }
}

SCENARIO("OptimisationProblem.getNumberOfDistinctEvaluations", "[OptimisationProblem][OptimisationProblem.getNumberOfDistinctEvaluations]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The optimisation problem was just initialised or reset") {
    THEN("Return 0") {
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
    }
  }

  WHEN("The optimisation problem was processed") {
    auto objectiveFunction = [](
        const arma::Col<double>& parameter) {
      return arma::accu(parameter % mant::range(1, parameter.n_elem));
    };
    optimisationProblem.setObjectiveFunction(objectiveFunction);

    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    arma::Mat<double> parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
    // Duplicated all parameters and shuffle them afterwards, to observe if it works as expected with/without caching.
    parameters = arma::shuffle(arma::join_rows(parameters, parameters));
    CAPTURE(parameters);

    AND_WHEN("Caching was disabled") {
      ::mant::isCachingSamples = false;

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      ::mant::isCachingSamples = false;

      THEN("Return the same as *.getNumberOfEvaluations*") {
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == optimisationProblem.getNumberOfEvaluations());
      }
    }

    AND_WHEN("Caching was enabled") {
      ::mant::isCachingSamples = true;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      ::mant::isCachingSamples = false;

      THEN("Return the number of discrete evaluations") {
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == numberOfParameters);
      }
    }
  }
}

SCENARIO("OptimisationProblem.reset", "[OptimisationProblem][OptimisationProblem.reset]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  WHEN("The optimisation problem was processed") {
    auto objectiveFunction = [&optimisationProblem](
        const arma::Col<double>& parameter_) {
        return arma::accu(parameter_);
    };
    optimisationProblem.setObjectiveFunction(objectiveFunction);

    // Constraints
    const arma::Col<double>& lowerBounds = SYNCHRONISED(continuousRandomNumbers(numberOfDimensions));
    optimisationProblem.setLowerBounds(lowerBounds);
    CAPTURE(lowerBounds);

    const arma::Col<double>& upperBounds = SYNCHRONISED(continuousRandomNumbers(numberOfDimensions));
    optimisationProblem.setUpperBounds(upperBounds);
    CAPTURE(upperBounds);

    // Parameter space modifiers
    const arma::Col<arma::uword>& parameterPermutation = SYNCHRONISED(mant::randomPermutationVector(numberOfDimensions));
    optimisationProblem.setParameterPermutation(parameterPermutation);
    CAPTURE(parameterPermutation);

    const arma::Col<double>& parameterScaling = SYNCHRONISED(continuousRandomNumbers(numberOfDimensions));
    optimisationProblem.setParameterScaling(parameterScaling);
    CAPTURE(parameterScaling);

    const arma::Col<double>& parameterTranslation = SYNCHRONISED(continuousRandomNumbers(numberOfDimensions));
    optimisationProblem.setParameterTranslation(parameterTranslation);
    CAPTURE(parameterTranslation);

    const arma::Mat<double>& parameterRotation = SYNCHRONISED(mant::randomRotationMatrix(numberOfDimensions));
    optimisationProblem.setParameterRotation(parameterRotation);
    CAPTURE(parameterRotation);

    // Objective value space modifiers
    const double objectiveValueScaling = SYNCHRONISED(continuousRandomNumber());
    optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
    CAPTURE(objectiveValueScaling);

    const double objectiveValueTranslation = SYNCHRONISED(continuousRandomNumber());
    optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
    CAPTURE(objectiveValueTranslation);

    // Minimal parameter distance
    const arma::Col<double>& minimalParameterDistance = arma::abs(continuousRandomNumbers(numberOfDimensions - 1));
    CAPTURE(minimalParameterDistance);

    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    arma::Mat<double> parameters = continuousRandomNumbers(numberOfDimensions, numberOfParameters);
    // Duplicated all parameters and shuffle them afterwards, to observe if it works as expected with/without caching.
    parameters = arma::shuffle(arma::join_rows(parameters, parameters));
    CAPTURE(parameters);

    ::mant::isCachingSamples = true;
    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      optimisationProblem.getObjectiveValue(parameters.col(n));
    }
    ::mant::isCachingSamples = false;

    THEN("Reset only the counters (number of (distinct) evaluations) and cache") {
      optimisationProblem.reset();

      CHECK(optimisationProblem.getCachedSamples().size() == 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveValueScaling * objectiveFunction(parameterRotation * (parameterScaling % parameter.elem(parameterPermutation) - parameterTranslation)) + objectiveValueTranslation));
      }
    }
  }
}
