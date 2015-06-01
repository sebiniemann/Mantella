namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class SchaffersF7Function : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit SchaffersF7Function(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

        arma::Mat<T> rotationQ_;

        U getSoftConstraintsValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

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
    SchaffersF7Function<T, U>::SchaffersF7Function(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T, typename U>
    void SchaffersF7Function<T, U>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T, typename U>
    U SchaffersF7Function<T, U>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return static_cast<U>(10.0L) * this->getBoundConstraintsValue(parameter);
    }

    template <typename T, typename U>
    U SchaffersF7Function<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& s = arma::square(parameterConditioning_ % (rotationQ_ * this->getAsymmetricParameter(static_cast<T>(0.5L), parameter)));
      const arma::Col<T>& z = arma::pow(s.head(s.n_elem - 1u) + s.tail(s.n_elem - 1u), static_cast<T>(0.25L));

      return std::pow(static_cast<U>(arma::mean(z % (static_cast<T>(1.0L) + arma::square(arma::sin(static_cast<T>(50.0L) * arma::pow(z, static_cast<T>(0.4L))))))), static_cast<U>(2.0L));
    }

    template <typename T, typename U>
    std::string SchaffersF7Function<T, U>::toString() const noexcept {
      return "bbob_schaffers_f7_function";
    }

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> SchaffersF7Function<T, U>::serialise() const noexcept {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark<T, T>::serialise();
      
      for(std::size_t n = 0u; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(rotationQ_(n)));
      }
      
      return serialisedOptimisationProblem;
    }

    template <typename T, typename U>
    void SchaffersF7Function<T, U>::deserialise(
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
