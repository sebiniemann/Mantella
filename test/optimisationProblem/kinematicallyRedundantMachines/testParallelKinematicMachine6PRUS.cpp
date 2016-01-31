// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

class TestParallelKinematicMachine6PRUS : public mant::krm::ParallelKinematicMachine6PRUS {
 public:
  using mant::krm::ParallelKinematicMachine6PRUS::ParallelKinematicMachine6PRUS;

  // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
  using mant::krm::ParallelKinematicMachine6PRUS::objectiveFunction_;
};

SCENARIO("krm::ParallelKinematicMachine6PRUS.numberOfDimensions_", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.numberOfDimensions_]") {
  mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;

  THEN("Return 6") {
    CHECK(optimisationProblem.numberOfDimensions_ == 6);
  }
}

SCENARIO("krm::ParallelKinematicMachine6PRUS.objectiveFunction_", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.objectiveFunction_]") {
  GIVEN("A trajectory") {
    TestParallelKinematicMachine6PRUS optimisationProblem;

    arma::Mat<double> endEffectorPoses;
    REQUIRE(endEffectorPoses.load(::rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachines/_endEffectorPoses_6x100.input"));
    CAPTURE(endEffectorPoses);

    const arma::Mat<double>::fixed<6, 1>& redundantJointsActuations = {0.009376303840997, 0.091501367086860, 0.092977707039855};
    CAPTURE(redundantJointsActuations);

    THEN("Return its objective value") {
      arma::Col<double> expectedPoseInaccuracies;
      REQUIRE(expectedPoseInaccuracies.load(::rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine6prus_100x1.expected"));
      CAPTURE(expectedPoseInaccuracies);

      for (arma::uword n = 0; n < endEffectorPoses.n_cols; ++n) {
        optimisationProblem.setEndEffectorTrajectory(endEffectorPoses.col(n));
        CHECK(optimisationProblem.objectiveFunction_(redundantJointsActuations) == Approx(expectedPoseInaccuracies(n)));
      }
    }
  }
}

SCENARIO("krm::ParallelKinematicMachine6PRUS.getLowerBounds", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.getLowerBounds]") {
  GIVEN("Default lower bounds") {
    mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;

    THEN("Return the default lower bounds (-0.6, ..., -0.6)^6") {
      IS_EQUAL(optimisationProblem.getLowerBounds(), arma::zeros<arma::Col<double>>(6) - 0.6);
    }
  }
}

SCENARIO("krm::ParallelKinematicMachine6PRUS.getUpperBounds", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.getUpperBounds]") {
  GIVEN("Default upper bounds") {
    mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;

    THEN("Return the default upper bounds (0.2, ..., 0.2)^6") {
      IS_EQUAL(optimisationProblem.getUpperBounds(), arma::zeros<arma::Col<double>>(6) + 0.2);
    }
  }
}

SCENARIO("krm::ParallelKinematicMachine6PRUS.getObjectiveFunctionName", "[krm::ParallelKinematicMachine6PRUS][krm::ParallelKinematicMachine6PRUS.getObjectiveFunctionName]") {
  mant::krm::ParallelKinematicMachine6PRUS optimisationProblem;

  THEN("Return the objective function name") {
    CHECK(optimisationProblem.getObjectiveFunctionName() == "KRPI Parallel Kinematic Machine 6PRUS");
  }
}
