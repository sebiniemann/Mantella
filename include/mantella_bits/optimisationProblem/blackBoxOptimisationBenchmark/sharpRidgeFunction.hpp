namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class SharpRidgeFunction : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit SharpRidgeFunction(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

        arma::Mat<T> rotationQ_;

        U getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;
        
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
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

    template <typename T, typename U>
    SharpRidgeFunction<T, U>::SharpRidgeFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T, typename U>
    void SharpRidgeFunction<T, U>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T, typename U>
    U SharpRidgeFunction<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = rotationQ_ * (parameterConditioning_ %  parameter);
      return std::pow(static_cast<U>(z(0)), static_cast<U>(2.0L)) + static_cast<U>(100.0L) * static_cast<U>(arma::norm(z.tail(z.n_elem - 1u)));
    }

    template <typename T, typename U>
    std::string SharpRidgeFunction<T, U>::toString() const noexcept {
      return "bbob_sharp_ridge_function";
    }

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> SharpRidgeFunction<T, U>::serialise() const noexcept {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark<T, T>::serialise();
      
      for(std::size_t n = 0u; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(rotationQ_(n)));
      }
      
      return serialisedOptimisationProblem;
    }

    template <typename T, typename U>
    void SharpRidgeFunction<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      rotationQ_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0u; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
      }
        
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
