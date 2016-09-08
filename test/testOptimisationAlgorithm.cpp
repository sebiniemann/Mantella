// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("OptimisationAlgorithm::OptimisationAlgorithm", "[OptimisationAlgorithm][OptimisationAlgorithm::OptimisationAlgorithm]") {
  THEN("Throw no exception") {
    CHECK_NOTHROW(mant::OptimisationAlgorithm optimisationAlgorithm);
  }
}

SCENARIO("OptimisationAlgorithm.setInitialisingFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.setInitialisingFunctions]") {
  GIVEN("A vector of functions") {
    WHEN("All functions are callable") {
      THEN("Throw no exception and keep all counters and results intact") {
        mant::bbob::SphereFunction optimisationProblem(2);
        mant::RandomSearch optimisationAlgorithm;
        optimisationAlgorithm.setMaximalNumberOfIterations(3);
        ::mant::isCachingSamples = true;
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem);
        ::mant::isCachingSamples = false;
        ::mant::isRecordingSampling = false;

        CHECK_NOTHROW(
            optimisationAlgorithm.setInitialisingFunctions(
                {{[](
                      const arma::uword numberOfDimensions_,
                      const arma::mat& initialParameters_) {
                    return initialParameters_;
                  },
                  "Test function"}}));

        CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
        CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
        CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
        CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getInitialisingFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.getInitialisingFunctions]") {
  THEN("Return an empty vector") {
    mant::OptimisationAlgorithm optimisationAlgorithm;
    CHECK(optimisationAlgorithm.getInitialisingFunctions().size() == 0);
  }
}

SCENARIO("OptimisationAlgorithm.setNextParametersFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.setNextParametersFunctions]") {
  GIVEN("A vector of functions") {
    WHEN("All functions are callable") {
      THEN("Throw no exception and keep all counters and results intact") {
        mant::bbob::SphereFunction optimisationProblem(2);
        mant::RandomSearch optimisationAlgorithm;
        optimisationAlgorithm.setMaximalNumberOfIterations(3);
        ::mant::isCachingSamples = true;
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem);
        ::mant::isCachingSamples = false;
        ::mant::isRecordingSampling = false;

        CHECK_NOTHROW(
            optimisationAlgorithm.setNextParametersFunctions(
                {{[](
                      const arma::uword numberOfDimensions_,
                      const arma::mat& parameters_,
                      const arma::rowvec& objectiveValues_,
                      const arma::rowvec& differences_) { return parameters_; },
                  "Test function"}}));

        CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
        CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
        CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
        CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getNextParametersFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.getNextParametersFunctions]") {
  THEN("Return an empty vector") {
    mant::OptimisationAlgorithm optimisationAlgorithm;
    CHECK(optimisationAlgorithm.getNextParametersFunctions().size() == 0);
  }
}

SCENARIO("OptimisationAlgorithm.setBoundariesHandlingFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.setBoundariesHandlingFunctions]") {
  GIVEN("A vector of functions") {
    WHEN("All functions are callable") {
      THEN("Throw no exception and keep all counters and results intact") {
        mant::bbob::SphereFunction optimisationProblem(2);
        mant::RandomSearch optimisationAlgorithm;
        optimisationAlgorithm.setMaximalNumberOfIterations(3);
        ::mant::isCachingSamples = true;
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem);
        ::mant::isCachingSamples = false;
        ::mant::isRecordingSampling = false;

        CHECK_NOTHROW(
            optimisationAlgorithm.setBoundariesHandlingFunctions(
                {{[](
                      const arma::mat& parameters_,
                      const arma::umat& isBelowLowerBound_,
                      const arma::umat& isAboveUpperBound_) {
                    return parameters_;
                  },
                  "Test function"}}));

        CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
        CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
        CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
        CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getBoundariesHandlingFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.getBoundariesHandlingFunctions]") {
  THEN("Return the 'Set parts of an parameter that are out of bound directly to to bound' function") {
    mant::OptimisationAlgorithm optimisationAlgorithm;
    CHECK(optimisationAlgorithm.getBoundariesHandlingFunctions().at(0).second == "Set parts of an parameter that are out of bound directly to to bound");
  }
}

SCENARIO("OptimisationAlgorithm.setIsStagnatingFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.setIsStagnatingFunctions]") {
  GIVEN("A vector of functions") {
    WHEN("All functions are callable") {
      THEN("Throw no exception and keep all counters and results intact") {
        mant::bbob::SphereFunction optimisationProblem(2);
        mant::RandomSearch optimisationAlgorithm;
        optimisationAlgorithm.setMaximalNumberOfIterations(3);
        ::mant::isCachingSamples = true;
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem);
        ::mant::isCachingSamples = false;
        ::mant::isRecordingSampling = false;

        CHECK_NOTHROW(
            optimisationAlgorithm.setIsStagnatingFunctions(
                {{[](
                      const arma::mat& parameters_,
                      const arma::rowvec& objectiveValues_,
                      const arma::rowvec& differences_) { return false; },
                  "Test function"}}));

        CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
        CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
        CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
        CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getIsStagnatingFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.getIsStagnatingFunctions]") {
  THEN("Return an empty vector") {
    mant::OptimisationAlgorithm optimisationAlgorithm;
    CHECK(optimisationAlgorithm.getIsStagnatingFunctions().size() == 0);
  }
}

SCENARIO("OptimisationAlgorithm.setRestartingFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.setRestartingFunctions]") {
  GIVEN("A vector of functions") {
    WHEN("All functions are callable") {
      THEN("Throw no exception and keep all counters and results intact") {
        mant::bbob::SphereFunction optimisationProblem(2);
        mant::RandomSearch optimisationAlgorithm;
        optimisationAlgorithm.setMaximalNumberOfIterations(3);
        ::mant::isCachingSamples = true;
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem);
        ::mant::isCachingSamples = false;
        ::mant::isRecordingSampling = false;

        CHECK_NOTHROW(
            optimisationAlgorithm.setRestartingFunctions(
                {{[](
                      const arma::uword numberOfDimensions_,
                      const arma::mat& parameters_,
                      const arma::rowvec& objectiveValues_,
                      const arma::rowvec& differences_) { return parameters_; },
                  "Test function"}}));

        CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
        CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
        CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
        CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getRestartingFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.getRestartingFunctions]") {
  THEN("Return the 'Place all parameters randomly within the bounds' function") {
    mant::OptimisationAlgorithm optimisationAlgorithm;
    CHECK(optimisationAlgorithm.getRestartingFunctions().at(0).second == "Place all parameters randomly within the bounds");
  }
}

SCENARIO("OptimisationAlgorithm.setCommunicationFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.setCommunicationFunctions]") {
  GIVEN("A vector of functions") {
    WHEN("All functions are callable") {
      THEN("Throw no exception and keep all counters and results intact") {
        mant::bbob::SphereFunction optimisationProblem(2);
        mant::RandomSearch optimisationAlgorithm;
        optimisationAlgorithm.setMaximalNumberOfIterations(3);
        ::mant::isCachingSamples = true;
        ::mant::isRecordingSampling = true;
        optimisationAlgorithm.optimise(optimisationProblem);
        ::mant::isCachingSamples = false;
        ::mant::isRecordingSampling = false;

        CHECK_NOTHROW(
            optimisationAlgorithm.setCommunicationFunctions(
                {{[](
                      const arma::uword numberOfDimensions_,
                      const arma::mat& parameters_,
                      const arma::rowvec& objectiveValues_,
                      const arma::rowvec& differences_) { return parameters_; },
                  "Test function"}}));

        CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
        CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
        CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
        CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.getCommunicationFunctions", "[OptimisationAlgorithm][OptimisationAlgorithm.getCommunicationFunctions]") {
  THEN("Return the 'Find the cluster-wide best found sample' function") {
    mant::OptimisationAlgorithm optimisationAlgorithm;
    CHECK(optimisationAlgorithm.getCommunicationFunctions().at(0).second == "Find the cluster-wide best found sample");
  }
}

SCENARIO("OptimisationAlgorithm.optimise", "[OptimisationAlgorithm][OptimisationAlgorithm.optimise]") {
  GIVEN("An optimisation problem and initial parameters") {
    WHEN("The initial parameters' number of dimensions is equal to the optimisation problem's number of dimensions and the next parameters functions vector is non-empty") {
      THEN("Reset the counter (excluding the optimisation problem), the results and optimise the problem afterwards") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions(
            {{[](
                  const arma::vec& parameter_) {
                return arma::accu(parameter_);
              },
              "Test function"}});

        mant::OptimisationAlgorithm optimisationAlgorithm;
        optimisationAlgorithm.setNextParametersFunctions(
            {{[](
                  const arma::uword numberOfDimensions_,
                  const arma::mat& parameters_,
                  const arma::rowvec& objectiveValues_,
                  const arma::rowvec& differences_) {
                return arma::vec({0.45, 0.45});
              },
              "Test function"}});
        optimisationAlgorithm.setMaximalNumberOfIterations(3);

        ::mant::isCachingSamples = true;
        ::mant::isRecordingSampling = true;
        CHECK_NOTHROW(optimisationAlgorithm.optimise(optimisationProblem, arma::mat({{1.0, 2.0}, {2.0, -3.0}})));
        ::mant::isCachingSamples = false;
        ::mant::isRecordingSampling = false;

        CHECK(optimisationAlgorithm.getUsedNumberOfIterations() == 3);
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 5);
        CHECK(optimisationAlgorithm.isTerminated() == true);
        CHECK(optimisationAlgorithm.isFinished() == false);
        CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() == Approx(-2.0));
        CHECK(arma::approx_equal(optimisationAlgorithm.getBestFoundParameter(), arma::vec({-1.0, -1.0}), "absdiff", ::mant::machinePrecision) == true);
        CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 5);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 3);
        CHECK(optimisationProblem.getCachedSamples().size() == 3);

        ::mant::isCachingSamples = true;
        ::mant::isRecordingSampling = true;
        // Ensures that the optimisation will finish after evaluating the initial parameters.
        optimisationAlgorithm.setAcceptableObjectiveValue(-1.0);
        CHECK_NOTHROW(optimisationAlgorithm.optimise(optimisationProblem, arma::mat({{2.0, -3.0}, {1.0, 2.0}})));
        ::mant::isCachingSamples = false;
        ::mant::isRecordingSampling = false;

        CHECK(optimisationAlgorithm.getUsedNumberOfIterations() == 0);
        CHECK(optimisationAlgorithm.getRecordedSampling().size() == 2);
        CHECK(optimisationAlgorithm.isTerminated() == false);
        CHECK(optimisationAlgorithm.isFinished() == true);
        CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() == Approx(-1.0));
        CHECK(arma::approx_equal(optimisationAlgorithm.getBestFoundParameter(), arma::vec({-3.0, 2.0}), "absdiff", ::mant::machinePrecision) == true);
        CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 7);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 5);
        CHECK(optimisationProblem.getCachedSamples().size() == 5);
      }
    }
  }
}

SCENARIO("OptimisationAlgorithm.setNumberOfCommunicationStalls", "[OptimisationAlgorithm][OptimisationAlgorithm.setNumberOfCommunicationStalls]") {
  GIVEN("An acceptable objective value") {
    THEN("Throw no exception and keep all counters and results intact") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      ::mant::isCachingSamples = true;
      ::mant::isRecordingSampling = true;
      optimisationAlgorithm.optimise(optimisationProblem);
      ::mant::isCachingSamples = false;
      ::mant::isRecordingSampling = false;

      CHECK_NOTHROW(optimisationAlgorithm.setNumberOfCommunicationStalls(2));

      CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
      CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
      CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
      CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
      CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
      CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
      CHECK(optimisationProblem.getCachedSamples().empty() == false);
    }
  }
}

SCENARIO("OptimisationAlgorithm.getNumberOfCommunicationStalls", "[OptimisationAlgorithm][OptimisationAlgorithm.getNumberOfCommunicationStalls]") {
  WHEN("The default number of communication stalls is unchanged") {
    THEN("Return 0") {
      mant::OptimisationAlgorithm optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getNumberOfCommunicationStalls() == 0);
    }
  }
}

SCENARIO("OptimisationAlgorithm.setAcceptableObjectiveValue", "[OptimisationAlgorithm][OptimisationAlgorithm.setAcceptableObjectiveValue]") {
  GIVEN("An acceptable objective value") {
    THEN("Throw no exception and keep all counters and results intact") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      ::mant::isCachingSamples = true;
      ::mant::isRecordingSampling = true;
      optimisationAlgorithm.optimise(optimisationProblem);
      ::mant::isCachingSamples = false;
      ::mant::isRecordingSampling = false;

      CHECK_NOTHROW(optimisationAlgorithm.setAcceptableObjectiveValue(-1.0));

      CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
      CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
      CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
      CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
      CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
      CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
      CHECK(optimisationProblem.getCachedSamples().empty() == false);
    }
  }
}

SCENARIO("OptimisationAlgorithm.getAcceptableObjectiveValue", "[OptimisationAlgorithm][OptimisationAlgorithm.getAcceptableObjectiveValue]") {
  WHEN("The default acceptable objective value is unchanged") {
    THEN("Return -infinity") {
      mant::OptimisationAlgorithm optimisationAlgorithm;

      CHECK(std::isfinite(optimisationAlgorithm.getAcceptableObjectiveValue()) == false);
      CHECK(optimisationAlgorithm.getAcceptableObjectiveValue() < 0.0);
    }
  }
}

SCENARIO("OptimisationAlgorithm.setMaximalNumberOfIterations", "[OptimisationAlgorithm][OptimisationAlgorithm.setMaximalNumberOfIterations]") {
  GIVEN("A maximal number of iterations") {
    THEN("Throw no exception and keep all counters and results intact") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      ::mant::isCachingSamples = true;
      ::mant::isRecordingSampling = true;
      optimisationAlgorithm.optimise(optimisationProblem);
      ::mant::isCachingSamples = false;
      ::mant::isRecordingSampling = false;

      CHECK_NOTHROW(optimisationAlgorithm.setMaximalNumberOfIterations(10));

      CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
      CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
      CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
      CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
      CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
      CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
      CHECK(optimisationProblem.getCachedSamples().empty() == false);
    }
  }
}

SCENARIO("OptimisationAlgorithm.getMaximalNumberOfIterations", "[OptimisationAlgorithm][OptimisationAlgorithm.getMaximalNumberOfIterations]") {
  WHEN("The default maximal number of iterations is unchanged") {
    THEN("Return the largest representable supported integer") {
      mant::OptimisationAlgorithm optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getMaximalNumberOfIterations() == std::numeric_limits<arma::uword>::max());
    }
  }
}

SCENARIO("OptimisationAlgorithm.setMaximalDuration", "[OptimisationAlgorithm][OptimisationAlgorithm.setMaximalDuration]") {
  GIVEN("A maximal duration") {
    THEN("Throw no exception and keep all counters and results intact") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      ::mant::isCachingSamples = true;
      ::mant::isRecordingSampling = true;
      optimisationAlgorithm.optimise(optimisationProblem);
      ::mant::isCachingSamples = false;
      ::mant::isRecordingSampling = false;

      CHECK_NOTHROW(optimisationAlgorithm.setMaximalDuration(std::chrono::microseconds(100)));

      CHECK(optimisationAlgorithm.getUsedNumberOfIterations() > 0);
      CHECK(optimisationAlgorithm.getRecordedSampling().empty() == false);
      CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() < std::numeric_limits<double>::infinity());
      CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == false);
      CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
      CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
      CHECK(optimisationProblem.getCachedSamples().empty() == false);
    }
  }
}

SCENARIO("OptimisationAlgorithm.getMaximalDuration", "[OptimisationAlgorithm][OptimisationAlgorithm.getMaximalDuration]") {
  WHEN("The default maximal duration is unchanged") {
    THEN("Return 1 second") {
      mant::OptimisationAlgorithm optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getMaximalDuration() == std::chrono::seconds(1));
    }
  }
}

SCENARIO("OptimisationAlgorithm.isFinished", "[OptimisationAlgorithm][OptimisationAlgorithm.isFinished]") {
  WHEN("The optimisation process finished") {
    THEN("Return true") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      optimisationAlgorithm.setAcceptableObjectiveValue(std::numeric_limits<double>::infinity());
      optimisationAlgorithm.optimise(optimisationProblem);

      CHECK(optimisationAlgorithm.isFinished() == true);
    }
  }

  WHEN("The optimisation process did not finished") {
    THEN("Return false") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      optimisationAlgorithm.optimise(optimisationProblem);

      CHECK(optimisationAlgorithm.isFinished() == false);
    }
  }
}

SCENARIO("OptimisationAlgorithm.isTerminated", "[OptimisationAlgorithm][OptimisationAlgorithm.isTerminated]") {
  WHEN("The optimisation process was terminated") {
    THEN("Return true") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      optimisationAlgorithm.optimise(optimisationProblem);

      CHECK(optimisationAlgorithm.isTerminated() == true);
    }
  }

  WHEN("The optimisation process was not terminated") {
    THEN("Return false") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      optimisationAlgorithm.setAcceptableObjectiveValue(std::numeric_limits<double>::infinity());
      optimisationAlgorithm.optimise(optimisationProblem);

      CHECK(optimisationAlgorithm.isTerminated() == false);
    }
  }
}

SCENARIO("OptimisationAlgorithm.getUsedNumberOfIterations", "[OptimisationAlgorithm][OptimisationAlgorithm.getUsedNumberOfIterations]") {
  WHEN("An optimisation was proceed") {
    THEN("Return the used number of iterations") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      optimisationAlgorithm.optimise(optimisationProblem);

      CHECK(optimisationAlgorithm.getUsedNumberOfIterations() == 3);
    }
  }

  WHEN("The optimisation algorithm was just initiated") {
    THEN("Return 0") {
      mant::OptimisationAlgorithm optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getUsedNumberOfIterations() == 0);
    }
  }
}

SCENARIO("OptimisationAlgorithm.getUsedDuration", "[OptimisationAlgorithm][OptimisationAlgorithm.getUsedDuration]") {
#if !defined(SUPPORT_MPI)
  WHEN("An optimisation was proceed") {
    THEN("Return the used duration") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalDuration(std::chrono::microseconds(10));
      optimisationAlgorithm.optimise(optimisationProblem);

      CHECK(optimisationAlgorithm.getUsedDuration() >= std::chrono::microseconds(10));
    }
  }
#endif

  WHEN("The optimisation algorithm was just initiated") {
    THEN("Return 0") {
      mant::OptimisationAlgorithm optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getUsedDuration() == std::chrono::microseconds(0));
    }
  }
}

SCENARIO("OptimisationAlgorithm.getBestFoundObjectiveValue", "[OptimisationAlgorithm][OptimisationAlgorithm.getBestFoundObjectiveValue]") {
  WHEN("An optimisation was proceed") {
    THEN("Return the best found objective value") {
      mant::OptimisationProblem optimisationProblem(2);
      optimisationProblem.setObjectiveFunctions(
          {{[](
                const arma::vec& parameter_) {
              return arma::accu(arma::abs(parameter_ - 2.0)) + 1.0;
            },
            "Test function"}});

      mant::OptimisationAlgorithm optimisationAlgorithm;
      optimisationAlgorithm.setNextParametersFunctions(
          {{[](
                const arma::uword numberOfDimensions_,
                const arma::mat& parameters_,
                const arma::rowvec& objectiveValues_,
                const arma::rowvec& differences_) {
              return arma::vec({0.6, 0.6});
            },
            "Test function"}});
      optimisationAlgorithm.setMaximalNumberOfIterations(3);

      optimisationAlgorithm.optimise(optimisationProblem, arma::mat({{1.0, 2.0}, {2.0, -3.0}}));
      CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() == Approx(1.0));
    }
  }

  WHEN("The optimisation algorithm was just initiated") {
    THEN("Return +infinity") {
      mant::OptimisationAlgorithm optimisationAlgorithm;

      CHECK(std::isinf(optimisationAlgorithm.getBestFoundObjectiveValue()) == true);
      CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() > 0.0);
    }
  }
}

SCENARIO("OptimisationAlgorithm.getBestFoundParameter", "[OptimisationAlgorithm][OptimisationAlgorithm.getBestFoundParameter]") {
  WHEN("An optimisation was proceed") {
    THEN("Return the best found parameter") {
      mant::OptimisationProblem optimisationProblem(2);
      optimisationProblem.setObjectiveFunctions(
          {{[](
                const arma::vec& parameter_) {
              return arma::accu(arma::abs(parameter_ - 2.0)) + 1.0;
            },
            "Test function"}});

      mant::OptimisationAlgorithm optimisationAlgorithm;
      optimisationAlgorithm.setNextParametersFunctions(
          {{[](
                const arma::uword numberOfDimensions_,
                const arma::mat& parameters_,
                const arma::rowvec& objectiveValues_,
                const arma::rowvec& differences_) {
              return arma::vec({0.6, 0.6});
            },
            "Test function"}});
      optimisationAlgorithm.setMaximalNumberOfIterations(3);

      optimisationAlgorithm.optimise(optimisationProblem, arma::mat({{1.0, 2.0}, {2.0, -3.0}}));
      CHECK(arma::approx_equal(optimisationAlgorithm.getBestFoundParameter(), arma::vec({2.0, 2.0}), "absdiff", ::mant::machinePrecision) == true);
    }
  }

  WHEN("The optimisation algorithm was just initiated") {
    THEN("Return an empty vector") {
      mant::OptimisationAlgorithm optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == true);
    }
  }
}

SCENARIO("OptimisationAlgorithm.getRecordedSampling", "[OptimisationAlgorithm][OptimisationAlgorithm.getRecordedSampling]") {
  WHEN("Recording samples is not supported") {
    THEN("Return an empty set") {
      mant::bbob::SphereFunction optimisationProblem(2);
      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(3);
      optimisationAlgorithm.optimise(optimisationProblem);

      CHECK(optimisationAlgorithm.getRecordedSampling().empty() == true);
    }
  }

  WHEN("Recording samples is supported") {
    THEN("Return all samples") {
      mant::OptimisationProblem optimisationProblem(2);
      optimisationProblem.setObjectiveFunctions(
          {{[](
                const arma::vec& parameter_) { return arma::accu(parameter_); },
            "Test function"}});

      mant::OptimisationAlgorithm optimisationAlgorithm;
      optimisationAlgorithm.setNextParametersFunctions(
          {{[](
                const arma::uword numberOfDimensions_,
                const arma::mat& parameters_,
                const arma::rowvec& objectiveValues_,
                const arma::rowvec& differences_) {
              return arma::vec({0.45, 0.45});
            },
            "Test function"}});
      optimisationAlgorithm.setMaximalNumberOfIterations(3);

      ::mant::isRecordingSampling = true;
      optimisationAlgorithm.optimise(optimisationProblem, arma::mat({{1.0, 2.0}, {2.0, -3.0}}));
      ::mant::isRecordingSampling = false;

      std::vector<std::pair<arma::vec, double>> expectedRecordedSampling = {{{1.0, 2.0}, 3.0}, {{2.0, -3.0}, -1.0}, {{-1.0, -1.0}, -2.0}, {{-1.0, -1.0}, -2.0}, {{-1.0, -1.0}, -2.0}};
      CHECK(optimisationAlgorithm.getRecordedSampling() == expectedRecordedSampling);
    }
  }
}

SCENARIO("OptimisationAlgorithm.reset", "[OptimisationAlgorithm][OptimisationAlgorithm.reset]") {
  THEN("Reset all counters and results while leaving everything else intact") {
    mant::bbob::SphereFunction optimisationProblem(2);
    mant::OptimisationAlgorithm optimisationAlgorithm;
    optimisationAlgorithm.setInitialisingFunctions(
        {{[](
              const arma::uword numberOfDimensions_,
              const arma::mat& initialParameters_) {
            return initialParameters_;
          },
          "Test function"}});
    optimisationAlgorithm.setNextParametersFunctions(
        {{[](
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_) { return parameters_; },
          "Test function"}});
    optimisationAlgorithm.setBoundariesHandlingFunctions(
        {{[](
              const arma::mat& parameters_, const arma::umat& isBelowLowerBound_, const arma::umat& isAboveUpperBound_) { return parameters_; },
          "Test function"}});
    optimisationAlgorithm.setIsStagnatingFunctions(
        {{[](
              const arma::mat& parameters_, const arma::rowvec& objectiveValues_, const arma::rowvec& differences_) { return false; },
          "Test function"}});
    optimisationAlgorithm.setRestartingFunctions(
        {{[](
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_) { return parameters_; },
          "Test function"}});
    optimisationAlgorithm.setCommunicationFunctions(
        {{[](
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_) { return parameters_; },
          "Test function"}});

    optimisationAlgorithm.setMaximalNumberOfIterations(3);
    ::mant::isCachingSamples = true;
    ::mant::isRecordingSampling = true;
    optimisationAlgorithm.optimise(optimisationProblem, arma::mat({{1.0, 2.0}, {2.0, -3.0}}));
    ::mant::isCachingSamples = false;
    ::mant::isRecordingSampling = false;

    CHECK_NOTHROW(optimisationAlgorithm.reset());

    CHECK(optimisationAlgorithm.getInitialisingFunctions().size() > 0);
    CHECK(optimisationAlgorithm.getNextParametersFunctions().size() > 0);
    CHECK(optimisationAlgorithm.getBoundariesHandlingFunctions().size() > 0);
    CHECK(optimisationAlgorithm.getIsStagnatingFunctions().size() > 0);
    CHECK(optimisationAlgorithm.getRestartingFunctions().size() > 0);
    CHECK(optimisationAlgorithm.getCommunicationFunctions().size() > 0);
    CHECK(optimisationAlgorithm.getUsedNumberOfIterations() == 0);
    CHECK(optimisationAlgorithm.getUsedDuration() == std::chrono::microseconds(0));
    CHECK(optimisationAlgorithm.getRecordedSampling().empty() == true);
    CHECK(optimisationAlgorithm.getBestFoundObjectiveValue() >= std::numeric_limits<double>::infinity());
    CHECK(optimisationAlgorithm.getBestFoundParameter().is_empty() == true);
    CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
    CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
    CHECK(optimisationProblem.getCachedSamples().empty() == false);
  }
}
