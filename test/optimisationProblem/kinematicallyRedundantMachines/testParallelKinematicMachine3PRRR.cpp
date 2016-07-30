// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("krm::ParallelKinematicMachine3PRRR.numberOfDimensions_", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.numberOfDimensions_]") {
  THEN("Return 3") {
    mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;
    CHECK(optimisationProblem.numberOfDimensions_ == 3);
  }
}

SCENARIO("krm::ParallelKinematicMachine3PRRR.numberOfWorkspaceDimensions_", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.numberOfWorkspaceDimensions_]") {
  THEN("Return 3") {
    mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;
    CHECK(optimisationProblem.numberOfWorkspaceDimensions_ == 3);
  }
}

SCENARIO("krm::ParallelKinematicMachine3PRRR.getLowerBounds", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.getLowerBounds]") {
  THEN("Return the default lower bounds (-0.5, -0.2, -0.2)") {
    mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;
    CHECK(arma::approx_equal(optimisationProblem.getLowerBounds(), arma::vec({-0.5, -0.2, -0.2}), "absdiff", ::mant::machinePrecision) == true);
  }
}

SCENARIO("krm::ParallelKinematicMachine3PRRR.getUpperBounds", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.getUpperBounds]") {
  THEN("Return the default upper bounds (0.5, 0.8, 0.8)") {
    mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;
    CHECK(arma::approx_equal(optimisationProblem.getUpperBounds(), arma::vec({0.5, 0.8, 0.8}), "absdiff", ::mant::machinePrecision) == true);
  }
}

SCENARIO("krm::ParallelKinematicMachine3PRRR.getObjectiveFunctions", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.getObjectiveFunctions]") {
  GIVEN("An redundant joints actuation and an end effector pose") {
    mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;
    optimisationProblem.setEndEffectorTrajectory(arma::vec({0.1, 0.05, 0.05}));

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({0.009376303840997, 0.091501367086860, 0.092977707039855}) == Approx(293.8775769234));
  }

  THEN("Return the objective function name") {
    mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "KRM Parallel Kinematic Machine 3PRRR");
  }
}
