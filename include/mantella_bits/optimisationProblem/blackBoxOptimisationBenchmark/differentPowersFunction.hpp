namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class DifferentPowersFunction : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit DifferentPowersFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
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
    DifferentPowersFunction<T, U>::DifferentPowersFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T, typename U>
    U DifferentPowersFunction<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = arma::abs(parameter);
      return static_cast<U>(arma::norm(z % this->getConditionedParameter(arma::square(z))));
    }

    template <typename T, typename U>
    std::string DifferentPowersFunction<T, U>::toString() const noexcept {
      return "bbob_different_powers_function";
    }
    
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> DifferentPowersFunction<T, U>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T, T>::serialise();
    }

    template <typename T, typename U>
    void DifferentPowersFunction<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}