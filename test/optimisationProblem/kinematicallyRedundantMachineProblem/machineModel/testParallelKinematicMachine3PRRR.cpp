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

class TestParallelKinematicMachine3PRRR : public mant::krmp::ParallelKinematicMachine3PRRR {
 public:
  using mant::krmp::ParallelKinematicMachine3PRRR::ParallelKinematicMachine3PRRR;

  // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
  using mant::krmp::ParallelKinematicMachine3PRRR::poseInaccuracyFunction_;
  using mant::krmp::ParallelKinematicMachine3PRRR::modelFunction_;
};

TEST_CASE("krmp::ParallelKinematicMachine3PRRR") {
  TestParallelKinematicMachine3PRRR machineModel(
      arma::join_slices(arma::Mat<double>::fixed<2, 3>({0.6, 1.0392, 0.0, 0.8, 1.2, 0.8}), arma::Mat<double>::fixed<2, 3>({0.6, 1.0392, 0.0, -0.2, 1.2, -0.2})),
      {0.6, 0.6, 0.6, 0.6, 0.6, 0.6},
      {-0.000066580445834, 0.106954081945581, -0.092751709777083, -0.053477040972790, 0.092818290222917, -0.053477040972790});

  SECTION(".poseInaccuracyFunction_") {
    SECTION("Returns the expected pose inaccuracy") {
      // *Note:* There is no need to *CAPTURE* anything below, as everything is stored on the hard disk, and easily accessible.

      arma::Mat<double> endEffectorPoses;
      REQUIRE(endEffectorPoses.load(rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachineProblem/_endEffectorPoses_3x100.input"));

      arma::Mat<double> redundantJointsActuations;
      REQUIRE(redundantJointsActuations.load(rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachineProblem/_redundantJointsActuations_100x2.input"));

      arma::Col<double> expectedPoseInaccuracy;
      REQUIRE(expectedPoseInaccuracy.load(rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachineProblem/parallelKinematicMachine3prrr_dim2_100x1.expected"));

      for (arma::uword n = 0; n < endEffectorPoses.n_cols; ++n) {
        CHECK(machineModel.poseInaccuracyFunction_(endEffectorPoses.col(n), redundantJointsActuations.row(n)) == Approx(expectedPoseInaccuracy(n)));
      }
    }
  }
}
