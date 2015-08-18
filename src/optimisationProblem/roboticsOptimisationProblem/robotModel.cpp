#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {
  namespace robotics {
    RobotModel::RobotModel(
        const arma::uword numberOfActiveJoints,
        const arma::uword numberOfRedundantJoints)
      : numberOfActiveJoints_(numberOfActiveJoints),
        numberOfRedundantJoints_(numberOfRedundantJoints) {
      setMinimalActiveJointsActuation(arma::zeros<arma::Col<double>>(numberOfActiveJoints_));
      setMaximalActiveJointsActuation(arma::ones<arma::Col<double>>(numberOfActiveJoints_));
      
      setMinimalRedundantJointsActuation(arma::zeros<arma::Col<double>>(numberOfRedundantJoints_));
      setMaximalRedundantJointsActuation(arma::ones<arma::Col<double>>(numberOfRedundantJoints_));
    } 
  
    void RobotModel::setMinimalActiveJointsActuation(
        const arma::Row<double> minimalActiveJointsActuation) {
      verify(minimalActiveJointsActuation.n_elem == numberOfActiveJoints_, "The number of actuated active joints must be equal to the number of active joints.");
      
      minimalActiveJointsActuation_ = minimalActiveJointsActuation;
    }
    
    void RobotModel::setMaximalActiveJointsActuation(
        const arma::Row<double> maximalActiveJointsActuation) {
      verify(maximalActiveJointsActuation.n_elem == numberOfActiveJoints_, "The number of actuated active joints must be equal to the number of active joints.");
      
      maximalActiveJointsActuation_ = maximalActiveJointsActuation;
    }

    arma::Row<double> RobotModel::getMinimalActiveJointsActuation() const {
      return minimalActiveJointsActuation_;
    }
    
    arma::Row<double> RobotModel::getMaximalActiveJointsActuation() const {
      return maximalActiveJointsActuation_;
    }

    void RobotModel::setMinimalRedundantJointsActuation(
        const arma::Row<double> minimalRedundantJointsActuation) {
      verify(minimalRedundantJointsActuation.n_elem == numberOfRedundantJoints_, "The number of actuated redundant joints must be equal to the number of redundant joints.");
      
      minimalRedundantJointsActuation_ = minimalRedundantJointsActuation;
    }
    
    void RobotModel::setMaximalRedundantJointsActuation(
        const arma::Row<double> maximalRedundantJointsActuation) {
      verify(maximalRedundantJointsActuation.n_elem == numberOfRedundantJoints_, "The number of actuated redundant joints must be equal to the number of redundant joints.");
      
      maximalRedundantJointsActuation_ = maximalRedundantJointsActuation;
    }

    arma::Row<double> RobotModel::getMinimalRedundantJointsActuation() const {
      return minimalRedundantJointsActuation_;
    }
    
    arma::Row<double> RobotModel::getMaximalRedundantJointsActuation() const {
      return maximalRedundantJointsActuation_;
    }
  
    arma::Cube<double> RobotModel::getModel(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      verify(redundantJointsActuation.n_elem == numberOfRedundantJoints_, "The number of actuated redundant joints must be equal to the number of redundant joints.");
      verify(!arma::any(redundantJointsActuation < minimalRedundantJointsActuation_) && !arma::any(redundantJointsActuation > maximalRedundantJointsActuation_), "The redundant joints must be actuated within the given bounds.");

      return getModelImplementation(endEffectorPose, redundantJointsActuation);
    }

    arma::Row<double> RobotModel::getActuation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      verify(redundantJointsActuation.n_elem == numberOfRedundantJoints_, "The number of actuated redundant joints must be equal to the number of redundant joints.");
      verify(!arma::any(redundantJointsActuation < minimalRedundantJointsActuation_) && !arma::any(redundantJointsActuation > maximalRedundantJointsActuation_), "The redundant joints must be actuated within the given bounds.");
      
      const arma::Row<double>& actuation = getActuationImplementation(endEffectorPose, redundantJointsActuation);
      assert(!any(actuation < minimalActiveJointsActuation_) && !any(actuation > maximalActiveJointsActuation_));
      
      return actuation;
    }

    double RobotModel::getEndEffectorPoseError(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      verify(redundantJointsActuation.n_elem == numberOfRedundantJoints_, "The number of actuated redundant joints must be equal to the number of redundant joints.");
      verify(!arma::any(redundantJointsActuation < minimalRedundantJointsActuation_) && !arma::any(redundantJointsActuation > maximalRedundantJointsActuation_), "The redundant joints must be actuated within the given bounds.");
      
      const double& endEffectorPoseError = getEndEffectorPoseErrorImplementation(endEffectorPose, redundantJointsActuation);
      assert(endEffectorPoseError >= 0);
      
      return endEffectorPoseError;
    }
  }
}