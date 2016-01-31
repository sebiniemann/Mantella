// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("krm::KinematicallyRedundantMachines.setEndEffectorTrajectory", "[krm::KinematicallyRedundantMachines][krm::KinematicallyRedundantMachines.setEndEffectorTrajectory]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::krm::KinematicallyRedundantMachines optimisationProblem(numberOfDimensions);

  const arma::uword numberOfPoses = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfPoses);

  GIVEN("An end-effector trajectory") {
    WHEN("The end-effector trajectory is finite and has exactly [numberOfDimensions] rows") {
      const arma::Mat<double>& endEffectorTrajectory = continuousRandomNumbers(numberOfDimensions, numberOfPoses);
      CAPTURE(endEffectorTrajectory);

      THEN("Throw no exception") {
        CHECK_NOTHROW(optimisationProblem.setEndEffectorTrajectory(endEffectorTrajectory));
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

        optimisationProblem.setEndEffectorTrajectory(endEffectorTrajectory);

        CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
        CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
        CHECK(optimisationProblem.getCachedSamples().size() == 0);
      }

#if defined(SUPPORT_MPI)
      THEN("The end-effector trajectory are synchronised over MPI") {
        optimisationProblem.setEndEffectorTrajectory(endEffectorTrajectory);
        IS_EQUAL(optimisationProblem.getEndEffectorTrajectory(), SYNCHRONISED(endEffectorTrajectory));
      }
#endif
    }

    WHEN("The end-effector trajectory has less then [numberOfDimensions] rows") {
      const arma::Mat<double>& endEffectorTrajectory = continuousRandomNumbers(numberOfDimensions - 1, numberOfPoses);
      CAPTURE(endEffectorTrajectory);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setEndEffectorTrajectory(endEffectorTrajectory), std::logic_error);
      }
    }

    WHEN("The end-effector trajectory has more then [numberOfDimensions] rows") {
      const arma::Mat<double>& endEffectorTrajectory = continuousRandomNumbers(numberOfDimensions + discreteRandomNumber(), numberOfPoses);
      CAPTURE(endEffectorTrajectory);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setEndEffectorTrajectory(endEffectorTrajectory), std::logic_error);
      }
    }

    WHEN("At least one end-effector trajectory has an infinite element") {
      arma::Mat<double> endEffectorTrajectory = continuousRandomNumbers(numberOfDimensions, numberOfPoses);
      endEffectorTrajectory(0) = arma::datum::inf;
      CAPTURE(endEffectorTrajectory);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(optimisationProblem.setEndEffectorTrajectory(endEffectorTrajectory), std::logic_error);
      }
    }
  }
}

SCENARIO("krm::KinematicallyRedundantMachines.getEndEffectorTrajectory", "[krm::KinematicallyRedundantMachines][krm::KinematicallyRedundantMachines.getEndEffectorTrajectory]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
  CAPTURE(numberOfDimensions);

  mant::krm::KinematicallyRedundantMachines optimisationProblem(numberOfDimensions);

  GIVEN("Default end-effector trajectory") {
    THEN("Return the default end-effector trajectory (0, ..., 0)^n") {
      IS_EQUAL(optimisationProblem.getEndEffectorTrajectory(), arma::zeros<arma::Col<double>>(numberOfDimensions));
    }
  }

  GIVEN("An updated end-effector trajectory") {
    const arma::uword numberOfPoses = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfPoses);

    const arma::Mat<double>& endEffectorTrajectory = continuousRandomNumbers(numberOfDimensions, numberOfPoses);
    CAPTURE(endEffectorTrajectory);

    optimisationProblem.setEndEffectorTrajectory(endEffectorTrajectory);

    THEN("Return the updated end-effector trajectory") {
      IS_EQUAL(optimisationProblem.getEndEffectorTrajectory(), SYNCHRONISED(endEffectorTrajectory));
    }
  }
}
