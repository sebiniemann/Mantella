// Catch
#include <catch.hpp>
#include "../catchHelper.hpp"

SCENARIO("krm::KinematicallyRedundantMachines::KinematicallyRedundantMachines", "[krm::KinematicallyRedundantMachines][krm::KinematicallyRedundantMachines::KinematicallyRedundantMachines]") {
  GIVEN("A number of problem dimensions and a number of workspace dimensions") {
    WHEN("The number of problem dimensions is 0") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::krm::KinematicallyRedundantMachines optimisationProblem(0, 1), std::domain_error);
      }
    }

    WHEN("The number of workspace dimensions is 0") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(mant::krm::KinematicallyRedundantMachines optimisationProblem(1, 0), std::domain_error);
      }
    }

    WHEN("The number of problem and workspace dimensions is greater than 0") {
      THEN("Throw no exception") {
        CHECK_NOTHROW(mant::krm::KinematicallyRedundantMachines optimisationProblem(2, 2));
      }
    }
  }
}

SCENARIO("krm::KinematicallyRedundantMachines.setEndEffectorTrajectory", "[krm::KinematicallyRedundantMachines][krm::KinematicallyRedundantMachines.setEndEffectorTrajectory]") {
  GIVEN("An end-effector trajectory") {
    WHEN("The trajectory is empty") {
      THEN("Throw an invalid argument error") {
        mant::krm::KinematicallyRedundantMachines optimisationProblem(3, 2);
        CHECK_THROWS_AS(optimisationProblem.setEndEffectorTrajectory({}), std::invalid_argument);
      }
    }

    WHEN("The trajectory's number of rows is unequal to the number of workspace dimensions") {
      THEN("Throw an invalid argument error") {
        mant::krm::KinematicallyRedundantMachines optimisationProblem(3, 2);
        CHECK_THROWS_AS(optimisationProblem.setEndEffectorTrajectory({{1.0, -2.0, 4.0}}), std::invalid_argument);
        CHECK_THROWS_AS(optimisationProblem.setEndEffectorTrajectory({{1.0, -2.0, 4.0}, {1.0, -3.0, 4.0}, {7.0, -6.0, 5.0}}), std::invalid_argument);
      }
    }

    WHEN("The trajectory's number of rows is equal to the number of workspace dimensions") {
      THEN("Throw no exception and reset all counters and caches") {
        mant::krm::KinematicallyRedundantMachines optimisationProblem(3, 2);
        optimisationProblem.setObjectiveFunctions({{[](
                                                        const arma::vec& parameter_) {
                                                      return arma::accu(parameter_);
                                                    },
            "Test function"}});

        ::mant::isCachingSamples = true;
        optimisationProblem.getObjectiveValue({1.0, -2.0, 3.0});
        ::mant::isCachingSamples = false;

        CHECK_NOTHROW(optimisationProblem.setEndEffectorTrajectory({{1.0, -2.0}, {1.0, 5.0}}));

        CHECK(optimisationProblem.getUsedNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getUsedNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().empty() == true);
      }
    }
  }
}

SCENARIO("krm::KinematicallyRedundantMachines.getEndEffectorTrajectory", "[krm::KinematicallyRedundantMachines][krm::KinematicallyRedundantMachines.getEndEffectorTrajectory]") {
  WHEN("The default end-effector trajectory is unchanged") {
    THEN("Return (0, ..., 0)") {
      mant::krm::KinematicallyRedundantMachines optimisationProblem(3, 2);
      CHECK(arma::approx_equal(optimisationProblem.getEndEffectorTrajectory(), arma::vec({0.0, 0.0}), "absdiff", ::mant::machinePrecision) == true);
    }
  }
}
