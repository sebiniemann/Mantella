namespace mant {
  namespace bbob {
    template <typename T = double>
    class StepEllipsoidalFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
      public:
        explicit StepEllipsoidalFunction(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> firstParameterConditioning_;
        const arma::Col<T> secondParameterConditioning_;

        arma::Mat<T> rotationQ_;

        double getSoftConstraintsValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

        double getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;
        
#if defined(MANTELLA_USE_MPI)
        // Grants direct access to the otherwise hidden .serialise() and .deserialise(...) methods.
        friend class OptimisationAlgorithm;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        std::vector<double> serialise() const noexcept;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        void deserialise(
            const std::vector<double>& serialisedOptimisationProblem);
#endif
    };

    //
    // Implementation
    //

    template <typename T>
    StepEllipsoidalFunction<T>::StepEllipsoidalFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        firstParameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))),
        secondParameterConditioning_(this->getParameterConditioning(static_cast<T>(100.0L))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    void StepEllipsoidalFunction<T>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T>
    double StepEllipsoidalFunction<T>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return this->getBoundConstraintsValue(parameter);
    }
    
    template <typename T>
    double StepEllipsoidalFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& s = firstParameterConditioning_ % parameter;

      arma::Col<T> z = s;
      for (std::size_t n = 0; n < z.n_elem; ++n) {
        const T& value = s(n);

        if (std::abs(value) > static_cast<T>(0.5L)) {
          z(n) = std::round(value);
        } else {
          z(n) = std::round(value * static_cast<T>(10.0L)) / static_cast<T>(10.0L);
        }
      }

      return 0.1 * std::max(std::abs(static_cast<double>(s(0))) / 10000.0, static_cast<double>(arma::dot(secondParameterConditioning_, arma::square(rotationQ_ * z))));
    }

    template <typename T>
    std::string StepEllipsoidalFunction<T>::toString() const noexcept {
      return "bbob_step_ellipsoidal_function";
    }

#if defined(MANTELLA_USE_MPI)
    template <typename T>
    std::vector<double> StepEllipsoidalFunction<T>::serialise() const noexcept {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark<T, T>::serialise();
      
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(rotationQ_(n)));
      }
      
      return serialisedOptimisationProblem;
    }

    template <typename T>
    void StepEllipsoidalFunction<T>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      rotationQ_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
      }
        
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
