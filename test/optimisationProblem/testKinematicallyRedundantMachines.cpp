// Catch
#include <catch.hpp>
#include "../catchHelper.hpp"

SCENARIO("krm::KinematicallyRedundantMachines::KinematicallyRedundantMachines", "[krm::KinematicallyRedundantMachines][krm::KinematicallyRedundantMachines::KinematicallyRedundantMachines]") {
  GIVEN("A number of problem dimensions and a number of workspace dimensions") {
    WHEN("The number of problem and workspace dimensions is greater than 0") {
      THEN("Throw no exception") {
        CHECK_NOTHROW(mant::krm::KinematicallyRedundantMachines optimisationProblem(2, 2));
      }
    }
  }
}

SCENARIO("krm::KinematicallyRedundantMachines.setEndEffectorTrajectory", "[krm::KinematicallyRedundantMachines][krm::KinematicallyRedundantMachines.setEndEffectorTrajectory]") {
  GIVEN("An end-effector trajectory") {
    WHEN("The trajectory's number of rows is equal to the number of workspace dimensions") {
      THEN("Throw no exception and reset all counters and caches") {
        mant::krm::KinematicallyRedundantMachines optimisationProblem(3, 2);
        optimisationProblem.setObjectiveFunctions(
            {{[](
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
