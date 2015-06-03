namespace mant {
  namespace robotic {
  template <typename T = double>
    class RobotModel : public Printable {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
      
      public:
        const std::size_t numberOfRedundantJoints_;
      
        arma::Cube<T> getModel(
            const arma::Col<T>& endEffectorPose,
            const arma::Row<T>& redundantJointActuations) const;

        arma::Row<T> getActuation(
            const arma::Col<T>& endEffectorPose,
            const arma::Row<T>& redundantJointActuations) const;

        arma::Col<T> getEndEffectorPose(
            const arma::Row<double>& actuations,
            const arma::Row<double>& redundantJointActuations) const;

        T getEndEffectorPoseError(
            const arma::Col<T>& endEffectorPose,
            const arma::Row<T>& redundantJointActuations) const;
        
      protected:
        virtual arma::Cube<T> getModelImplementation(
            const arma::Col<T>& endEffectorPose,
            const arma::Row<T>& redundantJointActuations) const = 0;

        virtual arma::Row<T> getActuationImplementation(
            const arma::Col<T>& endEffectorPose,
            const arma::Row<T>& redundantJointActuations) const = 0;

        virtual arma::Col<T> getEndEffectorPoseImplementation(
            const arma::Row<double>& actuations,
            const arma::Row<double>& redundantJointActuations) const = 0;

        virtual T getEndEffectorPoseErrorImplementation(
            const arma::Col<T>& endEffectorPose,
            const arma::Row<T>& redundantJointActuations) const noexcept = 0;
    };
    
    template <typename T>
    arma::Cube<T> RobotModel<T>::getModel(
        const arma::Col<T>& endEffectorPose,
        const arma::Row<T>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the redundantion joints actuation must be between [0, 1].");
      verify(redundantJointActuations.n_elem == numberOfRedundantJoints_, "The number of redundant joints actuation must be equal to the number of redundant joints.");
      
      return getModelImplementation(endEffectorPose, redundantJointActuations);
    }

    template <typename T>
    arma::Row<T> RobotModel<T>::getActuation(
        const arma::Col<T>& endEffectorPose,
        const arma::Row<T>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the redundantion joints actuation must be between [0, 1].");
      verify(redundantJointActuations.n_elem == numberOfRedundantJoints_, "The number of redundant joints actuation must be equal to the number of redundant joints.");
      
      return getActuationImplementation(endEffectorPose, redundantJointActuations);
    }

    template <typename T>
    arma::Col<T> RobotModel<T>::getEndEffectorPose(
        const arma::Row<double>& actuations,
        const arma::Row<double>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the redundantion joints actuation must be between [0, 1].");
      verify(redundantJointActuations.n_elem == numberOfRedundantJoints_, "The number of redundant joints actuation must be equal to the number of redundant joints.");
      
      return getEndEffectorPoseImplementation(actuations, redundantJointActuations);
    }

    template <typename T>
    T RobotModel<T>::getEndEffectorPoseError(
        const arma::Col<T>& endEffectorPose,
        const arma::Row<T>& redundantJointActuations) const {
      verify(arma::any(arma::vectorise(redundantJointActuations < 0)) || arma::any(arma::vectorise(redundantJointActuations > 1)), "All values for the redundantion joints actuation must be between [0, 1].");
      verify(redundantJointActuations.n_elem == numberOfRedundantJoints_, "The number of redundant joints actuation must be equal to the number of redundant joints.");
      
      const T& endEffectorPoseError = getEndEffectorPoseErrorImplementation(endEffectorPose, redundantJointActuations);
      
      assert(endEffectorPoseError >= 0);
      
      return endEffectorPoseError;
    }
  }
}