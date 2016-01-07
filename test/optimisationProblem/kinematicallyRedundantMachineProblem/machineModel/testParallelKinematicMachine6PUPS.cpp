// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

class TestParallelKinematicMachine6PUPS : public mant::krmp::ParallelKinematicMachine6PUPS {
 public:
  using mant::krmp::ParallelKinematicMachine6PUPS::ParallelKinematicMachine6PUPS;

  // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
  using mant::krmp::ParallelKinematicMachine6PUPS::poseInaccuracyFunction_;
  using mant::krmp::ParallelKinematicMachine6PUPS::modelFunction_;
};

TEST_CASE("krmp::ParallelKinematicMachine6PUPS") {
  TestParallelKinematicMachine6PUPS machineModel(
      arma::join_slices(arma::Mat<double>::fixed<3, 6>({-0.355455694558567, 0.354529254828353, -0.129903810567666, 0.463708870031622, 0.417029254828353, -0.346410161513775, 0.484759188345404, 0.130569033993384, -0.129903810567666, 0.129303493786838, -0.610098288821738, -0.346410161513775, -0.129303493786837, -0.610098288821738, -0.346410161513775, -0.593012363818459, 0.193069033993384, -0.346410161513775}), arma::Mat<double>::fixed<3, 6>({-0.355455694558567, 0.354529254828353, -0.129903810567666, 0.247202519085512, 0.292029254828353, 0.086602540378444, 0.484759188345404, 0.130569033993384, -0.129903810567666, 0.129303493786838, -0.360098288821738, 0.086602540378444, -0.129303493786837, -0.360098288821738, 0.086602540378444, -0.376506012872349, 0.068069033993384, 0.086602540378444})),
      {0.39, 0.95, 0.39, 0.95, 0.39, 0.95, 0.39, 0.95, 0.39, 0.95, 0.39, 0.95},
      {-0.025561381023353, 0.086293776138137, 0.12, 0.025561381023353, 0.086293776138137, 0.12, 0.087513292835791, -0.021010082747031, 0.12, 0.061951911812438, -0.065283693391106, 0.12, -0.061951911812438, -0.065283693391106, 0.12, -0.087513292835791, -0.021010082747032, 0.12});

  SECTION(".poseInaccuracyFunction_") {
    SECTION("Returns the expected pose inaccuracy") {
      // *Note:* There is no need to *CAPTURE* anything below, as everything is stored on the hard disk, and easily accessible.

      arma::Mat<double> endEffectorPoses;
      REQUIRE(endEffectorPoses.load(rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachineProblem/_endEffectorPoses_6x100.input"));

      arma::Mat<double> redundantJointsActuations;
      REQUIRE(redundantJointsActuations.load(rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachineProblem/_redundantJointsActuations_100x4.input"));

      arma::Col<double> expectedPoseInaccuracy;
      REQUIRE(expectedPoseInaccuracy.load(rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachineProblem/parallelKinematicMachine6pups_dim4_100x1.expected"));

      for (arma::uword n = 0; n < endEffectorPoses.n_cols; ++n) {
        CHECK(machineModel.poseInaccuracyFunction_(endEffectorPoses.col(n), redundantJointsActuations.row(n)) == Approx(expectedPoseInaccuracy(n)));
      }
    }
  }
}
