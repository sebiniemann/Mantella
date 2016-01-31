// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

class TestParallelKinematicMachine3PRRR : public mant::krm::ParallelKinematicMachine3PRRR {
 public:
  using mant::krm::ParallelKinematicMachine3PRRR::ParallelKinematicMachine3PRRR;

  // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
  using mant::krm::ParallelKinematicMachine3PRRR::objectiveFunction_;
};

SCENARIO("krm::ParallelKinematicMachine3PRRR.numberOfDimensions_", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.numberOfDimensions_]") {
  mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;

  THEN("Return 3") {
    CHECK(optimisationProblem.numberOfDimensions_ == 3);
  }
}

SCENARIO("krm::ParallelKinematicMachine3PRRR.objectiveFunction_", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.objectiveFunction_]") {
  GIVEN("A trajectory") {
    TestParallelKinematicMachine3PRRR optimisationProblem;

    arma::Mat<double> endEffectorPoses;
    REQUIRE(endEffectorPoses.load(::rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachines/_endEffectorPoses_3x100.input"));
    CAPTURE(endEffectorPoses);

    const arma::Mat<double>::fixed<3, 1>& redundantJointsActuations = {0.009376303840997, 0.091501367086860, 0.092977707039855};
    CAPTURE(redundantJointsActuations);

    THEN("Return its objective value") {
      arma::Col<double> expectedPoseInaccuracies;
      REQUIRE(expectedPoseInaccuracies.load(::rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachines/parallelKinematicMachine3prrr_100x1.expected"));
      CAPTURE(expectedPoseInaccuracies);

      for (arma::uword n = 0; n < endEffectorPoses.n_cols; ++n) {
        optimisationProblem.setEndEffectorTrajectory(endEffectorPoses.col(n));
        CHECK(optimisationProblem.objectiveFunction_(redundantJointsActuations) == Approx(expectedPoseInaccuracies(n)));
      }
    }
  }
}

SCENARIO("krm::ParallelKinematicMachine3PRRR.getLowerBounds", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.getLowerBounds]") {
  GIVEN("Default lower bounds") {
    mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;

    THEN("Return the default lower bounds (-0.5, -0.2, -0.2)") {
      IS_EQUAL(optimisationProblem.getLowerBounds(), {-0.5, -0.2, -0.2});
    }
  }
}

SCENARIO("krm::ParallelKinematicMachine3PRRR.getUpperBounds", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.getUpperBounds]") {
  GIVEN("Default upper bounds") {
    mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;

    THEN("Return the default upper bounds (0.5, 0.8, 0.8)") {
      IS_EQUAL(optimisationProblem.getUpperBounds(), {0.5, 0.8, 0.8});
    }
  }
}

SCENARIO("krm::ParallelKinematicMachine3PRRR.getObjectiveFunctionName", "[krm::ParallelKinematicMachine3PRRR][krm::ParallelKinematicMachine3PRRR.getObjectiveFunctionName]") {
  mant::krm::ParallelKinematicMachine3PRRR optimisationProblem;

  THEN("Return the objective function name") {
    CHECK(optimisationProblem.getObjectiveFunctionName() == "KRPI Parallel Kinematic Machine 3PRRR");
  }
}
