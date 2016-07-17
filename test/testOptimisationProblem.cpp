// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("OptimisationProblem.numberOfDimensions_", "[OptimisationProblem][OptimisationProblem.numberOfDimensions_]") {
  if (::nodeRank == 0) {
    mant::OptimisationProblem optimisationProblem(2);

    THEN("Return the first node's number of dimensions_") {
      CHECK(optimisationProblem.numberOfDimensions_ == 2);
    }
  } else {
    mant::OptimisationProblem optimisationProblem(4);

    THEN("Return the first node's number of dimensions_") {
      CHECK(optimisationProblem.numberOfDimensions_ == 2);
    }
  }
}

SCENARIO("OptimisationProblem::OptimisationProblem", "[OptimisationProblem][OptimisationProblem::OptimisationProblem]") {
  GIVEN("A number of dimensions") {
    WHEN("The number of dimensions is greater than 0") {
      THEN("Throw no exception") {
        CHECK_NOTHROW(mant::OptimisationProblem optimisationProblem(2));
      }
    }

    WHEN("The number of dimensions is 0") {
      THEN("Throw a logic error") {
        CHECK_THROWS_AS(mant::OptimisationProblem optimisationProblem(0), std::logic_error);
      }
    }
  }
}

SCENARIO("OptimisationProblem.setObjectiveFunctions", "[OptimisationProblem][OptimisationProblem.setObjectiveFunctions]") {
  GIVEN("A vector of named objective functions") {
    WHEN("An objective function is not callable") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setObjectiveFunctions({{nullptr, "Test function"}}), std::invalid_argument);
      }
    }

    WHEN("A objective function name is empty") {
      THEN("Throw no exception") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_NOTHROW(optimisationProblem.setObjectiveFunctions({{[](const arma::vec& parameter_) { return arma::accu(parameter_); }, ""}}));
      }
    }

    WHEN("The names are not unique") {
      THEN("Throw no exception") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_NOTHROW(optimisationProblem.setObjectiveFunctions({{[](const arma::vec& parameter_) { return arma::accu(parameter_); }, "Test function"}, {[](const arma::vec& parameter_) { return arma::prod(parameter_); }, "Test function"}}));
      }
    }

    WHEN("The objective function is callable") {
      THEN("Throw no exception and reset the cache and all counters") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_NOTHROW(optimisationProblem.setObjectiveFunctions({{[](const arma::vec& parameter_) { return arma::accu(parameter_); }, "Test function"}}));

        // Populates the cache and increments the counter
        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue({2.0, 1.0});
        ::mant::isCachingSamples = false;

        CHECK_NOTHROW(optimisationProblem.setObjectiveFunctions({{[](const arma::vec& parameter_) { return arma::prod(parameter_); }, "Test function"}}));

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == true);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getObjectiveFunctions", "[OptimisationProblem][OptimisationProblem.getObjectiveFunctions]") {
  WHEN("The default objective functions are unchanged") {
    THEN("Return an empty set") {
      mant::OptimisationProblem optimisationProblem(2);
      CHECK(optimisationProblem.getObjectiveFunctions().empty() == true);
    }
  }
}

SCENARIO("OptimisationProblem.getObjectiveValue", "[OptimisationProblem][OptimisationProblem.getObjectiveValue]") {
  GIVEN("A parameter") {
    WHEN("The parameter is empty") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        CHECK_THROWS_AS(optimisationProblem.getObjectiveValue({}), std::invalid_argument);
      }
    }

    WHEN("The parameters number of rows are unequal to the problem's number of dimensions") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        CHECK_THROWS_AS(optimisationProblem.getObjectiveValue({1.0}), std::invalid_argument);
      }
    }

    WHEN("The objective functions vector is empty") {
      THEN("Throw a logic error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.getObjectiveValue({1.0, -2.0}), std::invalid_argument);
      }
    }

    WHEN("The objective functions vector is non-empty") {
      THEN("Return the objective value, increase all counters and cache its value if supported") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        ::mant::isCachingSamples = true;
        CHECK(optimisationProblem.getObjectiveValue({1.0, -2.0}) == Approx(-1.0));
        ::mant::isCachingSamples = false;

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getObjectiveValueOfNormalisedParameter", "[OptimisationProblem][OptimisationProblem.getObjectiveValueOfNormalisedParameter]") {
  GIVEN("A parameter") {
    WHEN("The parameter is empty") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        CHECK_THROWS_AS(optimisationProblem.getObjectiveValueOfNormalisedParameter({}), std::invalid_argument);
      }
    }

    WHEN("The parameters number of rows are unequal to the problem's number of dimensions") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        CHECK_THROWS_AS(optimisationProblem.getObjectiveValueOfNormalisedParameter({1.0}), std::invalid_argument);
      }
    }

    WHEN("A lower bound is greater then the upper one") {
      THEN("Throw a logic error") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});
        optimisationProblem.setLowerBounds({2.0, 3.0});
        optimisationProblem.setUpperBounds({-2.0, 3.0});

        CHECK_THROWS_AS(optimisationProblem.getObjectiveValueOfNormalisedParameter({1.0}), std::logic_error);
      }
    }

    WHEN("The objective functions vector is empty") {
      THEN("Throw a logic error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.getObjectiveValueOfNormalisedParameter({1.0, -2.0}), std::invalid_argument);
      }
    }

    WHEN("The objective functions vector is non-empty") {
      THEN("Return the objective value, increase all counters and cache its value if supported") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        // The default bounds are set to -10 and 10.
        ::mant::isCachingSamples = true;
        CHECK(optimisationProblem.getObjectiveValueOfNormalisedParameter({1.0, -2.0}) == Approx(-40.0));
        ::mant::isCachingSamples = false;

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationProblem.setLowerBounds", "[OptimisationProblem][OptimisationProblem.setLowerBounds]") {
  GIVEN("Some lower bounds") {
    WHEN("The lower bounds are empty") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds({}), std::invalid_argument);
      }
    }

    WHEN("The number of lower bounds is unequal to the problem's number of dimension") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds({1.0}), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds({1.0, 2.0, 3.0}), std::invalid_argument);
      }
    }

    WHEN("The number of lower bounds are equal to the problem's number of dimension") {
      THEN("Throw no exception and keep all counters and caches intact") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue({1.0, -2.0});
        ::mant::isCachingSamples = false;

        CHECK_NOTHROW(optimisationProblem.setLowerBounds({std::numeric_limits<double>::quiet_NaN(), -std::numeric_limits<double>::infinity()}));

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getLowerBounds", "[OptimisationProblem][OptimisationProblem.getLowerBounds]") {
  WHEN("The default lower bounds are unchanged") {
    THEN("Return -10") {
      mant::OptimisationProblem optimisationProblem(2);
      CHECK(arma::approx_equal(optimisationProblem.getLowerBounds(), arma::vec({-10.0, -10.0}), "absdiff", ::mant::machinePrecision) == true);
    }
  }
}

SCENARIO("OptimisationProblem.setUpperBounds", "[OptimisationProblem][OptimisationProblem.setUpperBounds]") {
  GIVEN("Some upper bounds") {
    WHEN("The upper bounds are empty") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds({}), std::invalid_argument);
      }
    }

    WHEN("The number of upper bounds is unequal to the problem's number of dimension") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds({1.0}), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds({1.0, 2.0, 3.0}), std::invalid_argument);
      }
    }

    WHEN("The number of upper bounds are equal to the problem's number of dimension") {
      THEN("Throw no exception and keep all counters and caches intact") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue({1.0, -2.0});
        ::mant::isCachingSamples = false;

        CHECK_NOTHROW(optimisationProblem.setUpperBounds({std::numeric_limits<double>::quiet_NaN(), -std::numeric_limits<double>::infinity()}));

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getUpperBounds", "[OptimisationProblem][OptimisationProblem.getUpperBounds]") {
  WHEN("The default upper bounds are unchanged") {
    THEN("Return 10") {
      mant::OptimisationProblem optimisationProblem(2);
      CHECK(arma::approx_equal(optimisationProblem.getUpperBounds(), arma::vec({10.0, 10.0}), "absdiff", ::mant::machinePrecision) == true);
    }
  }
}

SCENARIO("OptimisationProblem.setParameterPermutation", "[OptimisationProblem][OptimisationProblem.setParameterPermutation]") {
  GIVEN("A parameter permutation") {
    WHEN("The permutation is empty") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation({}), std::invalid_argument);
      }
    }

    WHEN("Its not a permutation") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation({0, 3}), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation({0, 0}), std::invalid_argument);
      }
    }

    WHEN("The permutation's size is unequal to the problem's number of dimension") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(arma::uvec({0})), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(arma::uvec({0, 1, 2})), std::invalid_argument);
      }
    }

    WHEN("The permutation's size is equal to the problem's number of dimension") {
      THEN("Throw no exception and reset all counters and caches") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue({1.0, -2.0});
        ::mant::isCachingSamples = false;

        CHECK_NOTHROW(optimisationProblem.setParameterPermutation({1, 0}));

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == true);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getParameterPermutation", "[OptimisationProblem][OptimisationProblem.getParameterPermutation]") {
  WHEN("The default parameter permutation is unchanged") {
    THEN("Return (1, ..., n)") {
      mant::OptimisationProblem optimisationProblem(3);
      CHECK(arma::all(optimisationProblem.getParameterPermutation() == arma::uvec({0, 1, 2})) == true);
    }
  }
}

SCENARIO("OptimisationProblem.setParameterScaling", "[OptimisationProblem][OptimisationProblem.setParameterScaling]") {
  GIVEN("A parameter scaling") {
    WHEN("The scaling is empty") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setParameterScaling({}), std::invalid_argument);
      }
    }

    WHEN("The scaling's size is unequal to the problem's number of dimension") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(arma::vec({0.0})), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(arma::vec({0.0, 1.0, 2.0})), std::invalid_argument);
      }
    }

    WHEN("The scaling's size is equal to the problem's number of dimension") {
      THEN("Throw no exception and reset all counters and caches") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue({1.0, -2.0});
        ::mant::isCachingSamples = false;

        CHECK_NOTHROW(optimisationProblem.setParameterScaling({-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN()}));

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == true);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getParameterScaling", "[OptimisationProblem][OptimisationProblem.getParameterScaling]") {
  WHEN("The default parameter scaling is unchanged") {
    THEN("Return (1, ..., 1)") {
      mant::OptimisationProblem optimisationProblem(2);
      CHECK(arma::all(optimisationProblem.getParameterScaling() == arma::vec({1.0, 1.0})) == true);
    }
  }
}

SCENARIO("OptimisationProblem.setParameterTranslation", "[OptimisationProblem][OptimisationProblem.setParameterTranslation]") {
  GIVEN("A parameter translation") {
    WHEN("The translation is empty") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation({}), std::invalid_argument);
      }
    }

    WHEN("The translation's size is unequal to the problem's number of dimension") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(arma::vec({0.0})), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(arma::vec({0.0, 1.0, 2.0})), std::invalid_argument);
      }
    }

    WHEN("The translation's size is equal to the problem's number of dimension") {
      THEN("Throw no exception and reset all counters and caches") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue({1.0, -2.0});
        ::mant::isCachingSamples = false;

        CHECK_NOTHROW(optimisationProblem.setParameterTranslation({-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN()}));

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == true);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getParameterTranslation", "[OptimisationProblem][OptimisationProblem.getParameterTranslation]") {
  WHEN("The default parameter translation is unchanged") {
    THEN("Return (0, ..., 0)") {
      mant::OptimisationProblem optimisationProblem(2);
      CHECK(arma::all(optimisationProblem.getParameterTranslation() == arma::vec({0.0, 0.0})) == true);
    }
  }
}

SCENARIO("OptimisationProblem.setParameterRotation", "[OptimisationProblem][OptimisationProblem.setParameterRotation]") {
  GIVEN("A parameter rotation") {
    WHEN("The parameter is empty") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation({}), std::invalid_argument);
      }
    }

    WHEN("Its not a rotation matrix") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(arma::mat::fixed<1, 1>({1.0})), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(arma::mat({{1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}})), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(arma::mat({{1.0, 0.0}, {0.0, -2.0}})), std::invalid_argument);
      }
    }

    WHEN("The parameter's size is unequal to the problem's number of dimension") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(3);
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(mant::randomRotationMatrix(2)), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(mant::randomRotationMatrix(4)), std::invalid_argument);
      }
    }

    WHEN("The parameter's size is equal to the problem's number of dimension") {
      THEN("Throw no exception and reset all counters and caches") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue({1.0, -2.0});
        ::mant::isCachingSamples = false;

        CHECK_NOTHROW(optimisationProblem.setParameterRotation(mant::randomRotationMatrix(2)));

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == true);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getParameterRotation", "[OptimisationProblem][OptimisationProblem.getParameterRotation]") {
  WHEN("The default parameter rotation is unchanged") {
    THEN("Return an identity matrix") {
      mant::OptimisationProblem optimisationProblem(2);
      CHECK(arma::approx_equal(optimisationProblem.getParameterRotation(), arma::eye<arma::mat>(2, 2), "absdiff", ::mant::machinePrecision) == true);
    }
  }
}

SCENARIO("OptimisationProblem.setMinimalParameterDistance", "[OptimisationProblem][OptimisationProblem.setMinimalParameterDistance]") {
  GIVEN("A minimal parameter distance") {
    WHEN("The parameter is empty") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setMinimalParameterDistance({}), std::invalid_argument);
      }
    }

    WHEN("The parameter contains negative values") {
      THEN("Throw a domain error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setMinimalParameterDistance(arma::vec({1.0, -2.0})), std::domain_error);
      }
    }

    WHEN("The parameter's size is unequal to the problem's number of dimension") {
      THEN("Throw an invalid argument error") {
        mant::OptimisationProblem optimisationProblem(2);
        CHECK_THROWS_AS(optimisationProblem.setMinimalParameterDistance(arma::vec({0.0})), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setMinimalParameterDistance(arma::vec({0.0, 1.0, 2.0})), std::invalid_argument);
      }
    }

    WHEN("The parameter's size is equal to the problem's number of dimension") {
      THEN("Throw no exception and keep all counters and caches intact") {
        mant::OptimisationProblem optimisationProblem(2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue({1.0, -2.0});
        ::mant::isCachingSamples = false;

        CHECK_NOTHROW(optimisationProblem.setMinimalParameterDistance({std::numeric_limits<double>::infinity(), 0.0}));

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() > 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() > 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == false);
      }
    }
  }
}

SCENARIO("OptimisationProblem.getMinimalParameterDistance", "[OptimisationProblem][OptimisationProblem.getMinimalParameterDistance]") {
  WHEN("The default minimal parameter distance is unchanged") {
    THEN("Return (0, ..., 0)") {
      mant::OptimisationProblem optimisationProblem(2);
      CHECK(arma::approx_equal(optimisationProblem.getMinimalParameterDistance(), arma::vec({0.0, 0.0}), "absdiff", ::mant::machinePrecision) == true);
    }
  }
}

SCENARIO("OptimisationProblem.setObjectiveValueScaling", "[OptimisationProblem][OptimisationProblem.setObjectiveValueScaling]") {
  GIVEN("An objective value scaling") {
    THEN("Throw no exception and reset all counters and caches") {
      mant::OptimisationProblem optimisationProblem(2);
      optimisationProblem.setObjectiveFunctions({{[](
                                                      const arma::vec& parameter_) {
                                                    return arma::accu(parameter_);
                                                  },
          "Test function"}});

      ::mant::isCachingSamples = true;
      optimisationProblem.getObjectiveValue({1.0, -2.0});
      ::mant::isCachingSamples = false;

      CHECK_NOTHROW(optimisationProblem.setObjectiveValueScaling(std::numeric_limits<double>::quiet_NaN()));

      CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 0);
      CHECK(optimisationProblem.getCachedSamples().empty() == true);
    }
  }
}

SCENARIO("OptimisationProblem.getObjectiveValueScaling", "[OptimisationProblem][OptimisationProblem.getObjectiveValueScaling]") {
  WHEN("The default objective value scaling is unchanged") {
    THEN("Return 1") {
      mant::OptimisationProblem optimisationProblem(2);
      CHECK(optimisationProblem.getObjectiveValueScaling() == 1.0);
    }
  }
}

SCENARIO("OptimisationProblem.setObjectiveValueTranslation", "[OptimisationProblem][OptimisationProblem.setObjectiveValueTranslation]") {
  GIVEN("An objective value translation") {
    THEN("Throw no exception and reset all counters and caches") {
      mant::OptimisationProblem optimisationProblem(2);
      optimisationProblem.setObjectiveFunctions({{[](
                                                      const arma::vec& parameter_) {
                                                    return arma::accu(parameter_);
                                                  },
          "Test function"}});

      ::mant::isCachingSamples = true;
      optimisationProblem.getObjectiveValue({1.0, -2.0});
      ::mant::isCachingSamples = false;

      CHECK_NOTHROW(optimisationProblem.setObjectiveValueTranslation(std::numeric_limits<double>::quiet_NaN()));

      CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 0);
      CHECK(optimisationProblem.getCachedSamples().empty() == true);
    }
  }
}

SCENARIO("OptimisationProblem.getObjectiveValueTranslation", "[OptimisationProblem][OptimisationProblem.getObjectiveValueTranslation]") {
  WHEN("The default objective value translation is unchanged") {
    THEN("Return 1") {
      mant::OptimisationProblem optimisationProblem(2);
      CHECK(optimisationProblem.getObjectiveValueTranslation() == 0.0);
    }
  }
}

SCENARIO("OptimisationProblem.getCachedSamples", "[OptimisationProblem][OptimisationProblem.getCachedSamples]") {
  WHEN("Caching is not supported") {
    THEN("Return an empty set") {
      mant::OptimisationProblem optimisationProblem(2);
      optimisationProblem.setObjectiveFunctions({{[](
                                                      const arma::vec& parameter_) {
                                                    return arma::accu(parameter_);
                                                  },
          "Test function"}});

      // Caching is unsupported by default
      optimisationProblem.getObjectiveValue({1.0, -2.0});

      CHECK(optimisationProblem.getCachedSamples().empty() == true);
    }
  }

  WHEN("Caching is supported") {
    THEN("Return the cached samples") {
      mant::OptimisationProblem optimisationProblem(2);
      optimisationProblem.setObjectiveFunctions({{[](
                                                      const arma::vec& parameter_) {
                                                    return arma::accu(parameter_);
                                                  },
          "Test function"}});

      ::mant::isCachingSamples = true;
      optimisationProblem.getObjectiveValue({1.0, -2.0});
      optimisationProblem.getObjectiveValue({3.0, -3.0});
      optimisationProblem.getObjectiveValue({1.0, -2.0});
      ::mant::isCachingSamples = false;

      const std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>& expectedCachedSamples = {{{1.0, -2.0}, -1.0}, {{3.0, -3.0}, 0.0}};
      CHECK(optimisationProblem.getCachedSamples() == expectedCachedSamples);
    }
  }
}

SCENARIO("OptimisationProblem.getUsedNumberOfEvaluations", "[OptimisationProblem][OptimisationProblem.getUsedNumberOfEvaluations]") {
  THEN("Return the used number of evaluations") {
    mant::OptimisationProblem optimisationProblem(2);
    optimisationProblem.setObjectiveFunctions({{[](
                                                    const arma::vec& parameter_) {
                                                  return arma::accu(parameter_);
                                                },
        "Test function"}});

    ::mant::isCachingSamples = true;
    optimisationProblem.getObjectiveValue({1.0, -2.0});
    optimisationProblem.getObjectiveValue({3.0, -3.0});
    optimisationProblem.getObjectiveValue({1.0, -2.0});
    ::mant::isCachingSamples = false;

    CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 3);
  }
}

SCENARIO("OptimisationProblem.getUsedNumberOfDistinctEvaluations", "[OptimisationProblem][OptimisationProblem.getUsedNumberOfDistinctEvaluations]") {
  WHEN("Caching is not supported") {
    THEN("Return the same as `.getUsedNumberOfEvaluations()`") {
      mant::OptimisationProblem optimisationProblem(2);
      optimisationProblem.setObjectiveFunctions({{[](
                                                      const arma::vec& parameter_) {
                                                    return arma::accu(parameter_);
                                                  },
          "Test function"}});

      // Caching is unsupported by default
      optimisationProblem.getObjectiveValue({1.0, -2.0});
      optimisationProblem.getObjectiveValue({3.0, -3.0});
      optimisationProblem.getObjectiveValue({1.0, -2.0});

      CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == optimisationProblem.getUsedNumberOfEvaluations());
    }
  }

  WHEN("Caching is supported") {
    THEN("Return the  user number of distinct evaluations") {
      mant::OptimisationProblem optimisationProblem(2);
      optimisationProblem.setObjectiveFunctions({{[](
                                                      const arma::vec& parameter_) {
                                                    return arma::accu(parameter_);
                                                  },
          "Test function"}});

      ::mant::isCachingSamples = true;
      optimisationProblem.getObjectiveValue({1.0, -2.0});
      optimisationProblem.getObjectiveValue({3.0, -3.0});
      optimisationProblem.getObjectiveValue({1.0, -2.0});
      ::mant::isCachingSamples = false;

      CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 2);
    }
  }
}

SCENARIO("OptimisationProblem.reset", "[OptimisationProblem][OptimisationProblem.reset]") {
  THEN("Reset all counters and caches while leaving everything else intact") {
    mant::OptimisationProblem optimisationProblem(2);
    optimisationProblem.setObjectiveFunctions({{[](
                                                    const arma::vec& parameter_) {
                                                  return arma::accu(parameter_);
                                                },
        "Test function"}});

    optimisationProblem.setLowerBounds({1.0, -3.0});
    optimisationProblem.setUpperBounds({2.0, 3.0});
    optimisationProblem.setParameterPermutation({1, 0});
    optimisationProblem.setParameterScaling({-1.0, 3.0});
    optimisationProblem.setParameterTranslation({-1.0, 3.0});
    const arma::mat& randomMatrix = mant::synchronise(mant::randomRotationMatrix(2));
    optimisationProblem.setParameterRotation(randomMatrix);
    optimisationProblem.setMinimalParameterDistance({1.0, 3.0});
    optimisationProblem.setObjectiveValueScaling(3.0);
    optimisationProblem.setObjectiveValueTranslation(-2.0);

    optimisationProblem.reset();

    CHECK(arma::approx_equal(optimisationProblem.getLowerBounds(), arma::vec({1.0, -3.0}), "absdiff", ::mant::machinePrecision) == true);
    CHECK(arma::approx_equal(optimisationProblem.getUpperBounds(), arma::vec({2.0, 3.0}), "absdiff", ::mant::machinePrecision) == true);
    CHECK(arma::all(optimisationProblem.getParameterPermutation() == arma::uvec({1, 0})) == true);
    CHECK(arma::approx_equal(optimisationProblem.getParameterScaling(), arma::vec({-1.0, 3.0}), "absdiff", ::mant::machinePrecision) == true);
    CHECK(arma::approx_equal(optimisationProblem.getParameterTranslation(), arma::vec({-1.0, 3.0}), "absdiff", ::mant::machinePrecision) == true);
    CHECK(arma::approx_equal(optimisationProblem.getParameterRotation(), randomMatrix, "absdiff", ::mant::machinePrecision) == true);
    CHECK(arma::approx_equal(optimisationProblem.getMinimalParameterDistance(), arma::vec({1.0, 3.0}), "absdiff", ::mant::machinePrecision) == true);
    CHECK(optimisationProblem.getObjectiveValueScaling() == Approx(3.0));
    CHECK(optimisationProblem.getObjectiveValueTranslation() == Approx(-2.0));

    CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 0);
    CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 0);
    CHECK(optimisationProblem.getCachedSamples().empty() == true);

    CHECK_NOTHROW(optimisationProblem.getObjectiveValue(arma::vec({1.0, -1.0})));
  }
}
