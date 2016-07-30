// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("krm::ParallelKinematicMachine6PRUS.numberOfDimensions_", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.numberOfDimensions_]") {
  THEN("Return 6") {
    mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;

    CHECK(optimisationProblem.numberOfDimensions_ == 6);
  }
}

SCENARIO("krm::ParallelKinematicMachine6PRUS.numberOfWorkspaceDimensions_", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.numberOfWorkspaceDimensions_]") {
  THEN("Return 6") {
    mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;

    CHECK(optimisationProblem.numberOfWorkspaceDimensions_ == 6);
  }
}

SCENARIO("krm::ParallelKinematicMachine6PRUS.getLowerBounds", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.getLowerBounds]") {
  THEN("Return the default lower bounds (-0.6, ..., -0.6)^6") {
    mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;
    CHECK(arma::approx_equal(optimisationProblem.getLowerBounds(), arma::vec({-0.6, -0.6, -0.6, -0.6, -0.6, -0.6}), "absdiff", ::mant::machinePrecision) == true);
  }
}

SCENARIO("krm::ParallelKinematicMachine6PRUS.getUpperBounds", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.getUpperBounds]") {
  THEN("Return the default upper bounds (0.2, ..., 0.2)^6") {
    mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;
    CHECK(arma::approx_equal(optimisationProblem.getUpperBounds(), arma::vec({0.2, 0.2, 0.2, 0.2, 0.2, 0.2}), "absdiff", ::mant::machinePrecision) == true);
  }
}

SCENARIO("krm::ParallelKinematicMachine6PRUS.getObjectiveFunctions", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.getObjectiveFunctions]") {
  GIVEN("An redundant joints actuation and an end effector pose") {
    mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;
    optimisationProblem.setEndEffectorTrajectory(arma::vec({0.1, -0.1, 0.6, 0.05, -0.05, 0.1}));

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({0.009376303840997, 0.091501367086860, 0.092977707039855, -0.068477383664490, 0.094118556352123, 0.091433389648589}) == Approx(89.415928964));
  }

  THEN("Return the objective function name") {
    mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "KRM Parallel Kinematic Machine 6PRUS");
  }
}
