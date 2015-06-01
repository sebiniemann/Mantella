namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class KatsuuraFunction : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit KatsuuraFunction(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

        arma::Mat<T> rotationQ_;

        T getSoftConstraintsValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

        T getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;
        
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
        friend class OptimisationAlgorithm;
        
        std::vector<double> serialise() const noexcept;

        void deserialise(
            const std::vector<double>& serialisedOptimisationProblem);
#endif
    };

    //
    // Implementation
    //

    template <typename T, typename U>
    KatsuuraFunction<T, U>::KatsuuraFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(static_cast<T>(10.0L))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T, typename U>
    void KatsuuraFunction<T, U>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T, typename U>
    T KatsuuraFunction<T, U>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return this->getBoundConstraintsValue(parameter);
    }

    template <typename T, typename U>
    T KatsuuraFunction<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> z = rotationQ_ * (parameterConditioning_ % parameter);

      T product = static_cast<T>(1.0L);
      for (std::size_t n = 0; n < z.n_elem; ++n) {
          const T& value = z(n);

          T sum = static_cast<T>(0.0L);
          for (std::size_t k = 1; k < 33; ++k) {
              const T& power = std::pow(static_cast<T>(2.0L), k);
              sum += std::abs(power * value - std::round(power * value)) / power;
          }

          product *= std::pow(static_cast<T>(1.0L) + (static_cast<T>(n) + static_cast<T>(1.0L)) * sum, (static_cast<T>(10.0L) / std::pow(static_cast<T>(this->numberOfDimensions_), static_cast<T>(1.2L))));
      }

      return static_cast<T>(10.0L) / std::pow(static_cast<T>(this->numberOfDimensions_), static_cast<T>(2.0L)) * (product - static_cast<T>(1.0L));
    }

    template <typename T, typename U>
    std::string KatsuuraFunction<T, U>::toString() const noexcept {
      return "bbob_katsuura_function";
    }

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> KatsuuraFunction<T, U>::serialise() const noexcept {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark<T, T>::serialise();
      
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }
      
      return serialisedOptimisationProblem;
    }

    template <typename T, typename U>
    void KatsuuraFunction<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      rotationQ_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = serialisedOptimisationProblem.pop_back();
      }
        
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
