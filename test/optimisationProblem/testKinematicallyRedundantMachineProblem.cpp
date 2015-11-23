// Catch
#include <catch.hpp>

// C++ standard library
#include <memory>

// Mantella
#include <mantella>

TEST_CASE("KinematicallyRedundantMachineProblem") {
  SECTION("Works with any machine model.") {
    CHECK_NOTHROW(mant::krmp::KinematicallyRedundantMachineProblem optimisationProblem(std::shared_ptr<mant::krmp::MachineModel>(new mant::krmp::ParallelKinematicMachine3PRPR)));
  }
}
