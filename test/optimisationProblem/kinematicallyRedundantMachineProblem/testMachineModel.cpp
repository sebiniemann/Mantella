// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

TEST_CASE("MachineModel") {
  arma::uword numberOfActiveJoints = getDiscreteRandomNumber();
  CAPTURE(numberOfActiveJoints);
  arma::uword numberOfRedundantJoints = getDiscreteRandomNumber();
  CAPTURE(numberOfRedundantJoints);
  
  mant::krmp::MachineModel machineModel(numberOfActiveJoints, numberOfRedundantJoints);
  
  SECTION(".numberOfActiveJoints_") {
    SECTION("Is parametrisable by the constructor.") {
      CHECK(machineModel.numberOfActiveJoints_ == numberOfActiveJoints);
    }
  }
  
  SECTION(".numberOfRedundantJoints_") {
    SECTION("Is parametrisable by the constructor.") {
      CHECK(machineModel.numberOfRedundantJoints_ == numberOfRedundantJoints);
    }
  }
  
  SECTION(".setModelFunction") {
    auto modelFunction = [] (
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) {
      return arma::ones<arma::Cube<double>>(3, 4, 5);
    };
    
    const arma::Col<double> endEffectorPose = getContinuousRandomNumbers(getDiscreteRandomNumber());
    CAPTURE(endEffectorPose);
    const arma::Row<double> redundantJointsActuation = getContinuousRandomNumbers(numberOfRedundantJoints).t();
    CAPTURE(redundantJointsActuation);
      
    SECTION("Works with any model function.") {
      machineModel.setModelFunction(modelFunction);
      IS_EQUAL(machineModel.getModel(endEffectorPose, redundantJointsActuation), modelFunction(endEffectorPose, redundantJointsActuation));
    }
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if no callable function is set.") {
        CHECK_THROWS_AS(machineModel.getModel(endEffectorPose, redundantJointsActuation), std::logic_error);
      }
    }
  }
  
  SECTION(".getModel") {
    // This is already covered by *SECTION(".setModelFunction")*.
  }
  
  SECTION(".setPoseInaccuracyFunction") {
    auto poseInaccuracyFunction = [] (
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) {
      // *Note:* This is not a useful pose inaccuracy function ...
      return std::abs(arma::accu(endEffectorPose) + arma::prod(redundantJointsActuation));
    };
    
    const arma::Col<double> endEffectorPose = getContinuousRandomNumbers(getDiscreteRandomNumber());
    CAPTURE(endEffectorPose);
    const arma::Row<double> redundantJointsActuation = getContinuousRandomNumbers(numberOfRedundantJoints).t();
    CAPTURE(redundantJointsActuation);
      
    SECTION("Works with any model function.") {
      machineModel.setPoseInaccuracyFunction(poseInaccuracyFunction);
      CHECK(machineModel.getPoseInaccuracy(endEffectorPose, redundantJointsActuation) == Approx(poseInaccuracyFunction(endEffectorPose, redundantJointsActuation)));
    }
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if no callable function is set.") {
        CHECK_THROWS_AS(machineModel.getPoseInaccuracy(endEffectorPose, redundantJointsActuation), std::logic_error);
      }
    }
  }
  
  SECTION(".getPoseInaccuracy") {
    // This is already covered by *SECTION(".setPoseInaccuracyFunction")*.
  }
}
