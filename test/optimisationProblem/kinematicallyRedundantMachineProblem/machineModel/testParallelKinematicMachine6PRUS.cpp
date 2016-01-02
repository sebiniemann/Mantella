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

class TestParallelKinematicMachine6PRUS : public mant::krmp::ParallelKinematicMachine6PRUS {
  public:
    using mant::krmp::ParallelKinematicMachine6PRUS::ParallelKinematicMachine6PRUS;
    
    // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
    using mant::krmp::ParallelKinematicMachine6PRUS::poseInaccuracyFunction_;
    using mant::krmp::ParallelKinematicMachine6PRUS::modelFunction_;
};

TEST_CASE("krmp::ParallelKinematicMachine6PRUS") {
  TestParallelKinematicMachine6PRUS machineModel(
    arma::join_slices(arma::Mat<double>::fixed<3, 6>({-0.050659008749464, 0.360457577021932, -0.2, 0.050659008749464, 0.360457577021932, -0.6, 0.337494923062311, -0.136356800003392, -0.2, 0.286835914312847, -0.224100777018540, -0.6, -0.286835914312847, -0.224100777018540, -0.6, -0.337494923062311, -0.136356800003392, -0.6}), arma::Mat<double>::fixed<3, 6>({-0.050659008749464, 0.360457577021932, -0.2, 0.050659008749464, 0.360457577021932, 0.2, 0.337494923062311, -0.136356800003392, -0.2, 0.286835914312847, -0.224100777018540, 0.2, -0.286835914312847, -0.224100777018540, 0.2, -0.337494923062311, -0.136356800003392, 0.2})),
    {0.927183854566785, 0.374606593415919, 0, 0.927183854566785, -0.374606593415919, 0, -0.139173100960068, -0.990268068741570, 0, -0.788010753606724, -0.615661475325656, 0, -0.788010753606724, 0.615661475325656, 0, -0.139173100960068, 0.990268068741570, 0},
    {0.24, 0.56, 0.24, 0.56, 0.24, 0.56, 0.24, 0.56, 0.24, 0.56, 0.24, 0.56},
    {-0.027346281319362, 0.067684421383375, 0.0, 0.027346281319362, 0.067684421383375, 0.0, 0.072289569018135, -0.010159636370085, 0.0, 0.044943287698773, -0.057524785013291, 0.0, -0.044943287698773, -0.057524785013291, 0.0, -0.072289569018135, -0.010159636370085, 0.0});
  
  SECTION(".poseInaccuracyFunction_") {
    SECTION("Returns the expected pose inaccuracy") {
      // *Note:* There is no need to *CAPTURE* anything below, as everything is stored on the hard disk, and easily accessible.
      
      arma::Mat<double> endEffectorPoses;
      REQUIRE(endEffectorPoses.load(rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachineProblem/_endEffectorPoses_6x100.input"));
      
      arma::Mat<double> redundantJointsActuations;
      REQUIRE(redundantJointsActuations.load(rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachineProblem/_redundantJointsActuations_100x4.input"));

      arma::Col<double> expectedPoseInaccuracy;
      REQUIRE(expectedPoseInaccuracy.load(rootTestDataDirectory + "/optimisationProblem/kinematicallyRedundantMachineProblem/parallelKinematicMachine6prus_dim4_100x1.expected"));

      for (arma::uword n = 0; n < endEffectorPoses.n_cols; ++n) {
        CHECK(machineModel.poseInaccuracyFunction_(endEffectorPoses.col(n), redundantJointsActuations.row(n)) == Approx(expectedPoseInaccuracy(n)));
      }
    }
  }
}
