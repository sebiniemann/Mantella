#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel/parallelKinematicMachine3PUPS.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/geometry.hpp>

namespace mant {
  namespace robotics {
    ParallelKinematicMachine3PUPS::ParallelKinematicMachine3PUPS(
        const arma::uword numberOfRedundantJoints) 
      : RobotModel(3, numberOfRedundantJoints) {
      setMinimalActiveJointsActuation({0.39, 0.39, 0.39});
      setMaximalActiveJointsActuation({0.95, 0.95, 0.95});

      setEndEffectorJointPositions({
        -0.025561381023353, 0.086293776138137, 0.12,
        0.087513292835791, -0.021010082747031, 0.12,
        -0.061951911812438, -0.065283693391106, 0.12});

      setRedundantJointStartPositions({
        -0.463708870031622, 0.417029254828353, -0.346410161513775,
        0.593012363818459, 0.193069033993384, -0.346410161513775,
        -0.129303493786837, -0.610098288821738, -0.346410161513775});

      setRedundantJointEndPositions({
        -0.247202519085512, 0.292029254828353, 0.086602540378444,
        0.376506012872349, 0.068069033993384, 0.086602540378444,
        -0.129303493786837, -0.360098288821738, 0.086602540378444});

      redundantJointStartToEndPositions_ = redundantJointEndPositions_ - redundantJointStartPositions_;
      redundantJointIndicies_ = arma::find(arma::any(redundantJointStartToEndPositions_));

      redundantJointRotationAngles_.set_size(3, redundantJointIndicies_.n_elem);

      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const double& redundantJointXAngle = std::atan2(redundantJointStartToEndPositions_(1, n), redundantJointStartToEndPositions_(0, n));
        const double& redundantJointYAngle = std::atan2(redundantJointStartToEndPositions_(2, n), redundantJointStartToEndPositions_(1, n));
        redundantJointRotationAngles_.col(n) = arma::Col<double>({std::cos(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointXAngle) * std::cos(redundantJointYAngle), std::sin(redundantJointYAngle)});
      }
    }
            
    void ParallelKinematicMachine3PUPS::setEndEffectorJointPositions(
        arma::Mat<double>::fixed<3, 3> endEffectorJointPositions) {
      endEffectorJointPositions_ = endEffectorJointPositions;
    }
    
    void ParallelKinematicMachine3PUPS::setRedundantJointStartPositions(
        arma::Mat<double>::fixed<3, 3> redundantJointStartPositions) {
      redundantJointStartPositions_ = redundantJointStartPositions;
    }
    
    void ParallelKinematicMachine3PUPS::setRedundantJointEndPositions(
        arma::Mat<double>::fixed<3, 3> redundantJointEndPositions) {
      redundantJointEndPositions_ = redundantJointEndPositions;
    }
    
    arma::Mat<double>::fixed<3, 3> ParallelKinematicMachine3PUPS::getEndEffectorJointPositions() const {
      return endEffectorJointPositions_;
    }
    
    arma::Mat<double>::fixed<3, 3> ParallelKinematicMachine3PUPS::getRedundantJointStartPositions() const{
      return redundantJointStartPositions_;
    }
    
    arma::Mat<double>::fixed<3, 3> ParallelKinematicMachine3PUPS::getRedundantJointEndPositions() const{
      return redundantJointEndPositions_;
    }

    arma::Cube<double> ParallelKinematicMachine3PUPS::getModelImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < minimalRedundantJointsActuation_) && !arma::any(redundantJointsActuation > maximalRedundantJointsActuation_));

      arma::Cube<double> model;

      const arma::Col<double>& endEffectorPosition = endEffectorPose.subvec(0, 2);
      const double& endEffectorRollAngle = endEffectorPose(3);
      const double& endEffectorPitchAngle = endEffectorPose(4);
      const double& endEffectorYawAngle = endEffectorPose(5);

      model.slice(0) = redundantJointStartPositions_;
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const arma::uword& redundantJointIndex = redundantJointIndicies_(n);
        model.slice(0).col(redundantJointIndex) += redundantJointsActuation(redundantJointIndex) * redundantJointStartToEndPositions_.col(redundantJointIndex);
      }

      model.slice(1) = get3DRotation(endEffectorRollAngle, endEffectorPitchAngle, endEffectorYawAngle) * endEffectorJointPositions_;
      model.slice(1).each_col() += endEffectorPosition;

      return model;
    }

    arma::Row<double> ParallelKinematicMachine3PUPS::getActuationImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < minimalRedundantJointsActuation_) && !arma::any(redundantJointsActuation > maximalRedundantJointsActuation_));
      
      const arma::Cube<double>& model = getModel(endEffectorPose, redundantJointsActuation);

      const arma::Mat<double>& baseJoints = model.slice(0);
      const arma::Mat<double>& endEffectorJoints = model.slice(1);

      return arma::sqrt(arma::sum(arma::square(endEffectorJoints - baseJoints)));
    }

    double ParallelKinematicMachine3PUPS::getEndEffectorPoseErrorImplementation(
        const arma::Col<double>& endEffectorPose,
        const arma::Row<double>& redundantJointsActuation) const {
      assert(redundantJointsActuation.n_elem == numberOfRedundantJoints_);
      assert(!arma::any(redundantJointsActuation < minimalRedundantJointsActuation_) && !arma::any(redundantJointsActuation > maximalRedundantJointsActuation_));
      
      const arma::Cube<double>& model = getModel(endEffectorPose, redundantJointsActuation);

      const arma::Mat<double>& baseJoints = model.slice(0);

      const arma::Mat<double>& endEffectorJoints = model.slice(1);
      arma::Mat<double> endEffectorJointsRotated = endEffectorJoints;
      endEffectorJointsRotated.each_col() -= endEffectorPose.subvec(0, 1);

      const arma::Mat<double>& baseToEndEffectorJointPositions = endEffectorJoints - baseJoints;
      const arma::Row<double>& baseToEndEffectorJointsActuation = arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));

      if (arma::any(baseToEndEffectorJointsActuation < minimalActiveJointsActuation_) || arma::any(baseToEndEffectorJointsActuation > maximalActiveJointsActuation_)) {
        return 0.0;
      }

      arma::Mat<double> forwardKinematic;
      forwardKinematic.head_rows(3) = baseToEndEffectorJointPositions;
      for (arma::uword n = 0; n < baseToEndEffectorJointPositions.n_cols; ++n) {
        forwardKinematic.submat(3, n, 5, n) = arma::cross(endEffectorJointsRotated.col(n), baseToEndEffectorJointPositions.col(n));
      }

      arma::Mat<double> inverseKinematic(6, 3 + redundantJointIndicies_.n_elem, arma::fill::zeros);
      inverseKinematic.diag() = -arma::sqrt(arma::sum(arma::square(baseToEndEffectorJointPositions)));
      for (arma::uword n = 0; n < redundantJointIndicies_.n_elem; ++n) {
        const arma::uword& redundantJointIndex = redundantJointIndicies_(n);
        inverseKinematic(n, 3 + n) = arma::dot(baseToEndEffectorJointPositions.col(redundantJointIndex), redundantJointRotationAngles_.col(redundantJointIndex));
      }

      return -1.0 / arma::cond(arma::solve(forwardKinematic.t(), inverseKinematic));
    }
    
    std::string ParallelKinematicMachine3PUPS::toString() const {
      return "robotics_parallel_kinematic_machine_3pups";
    }
  }
}
