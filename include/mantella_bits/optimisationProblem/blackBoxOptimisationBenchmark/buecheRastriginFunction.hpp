namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class BuecheRastriginFunction : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit BuecheRastriginFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

        U getSoftConstraintsValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

        U getObjectiveValueImplementation(
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

    template <typename T, typename U>
    BuecheRastriginFunction<T, U>::BuecheRastriginFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      arma::Col<T> parameterTranslation = this->getRandomParameterTranslation();
      for (std::size_t n = 0; n < parameterTranslation.n_elem; n += 2) {
        parameterTranslation(n) = std::abs(parameterTranslation(n));
      }
      this->setParameterTranslation(parameterTranslation);
    }

    template <typename T, typename U>
    U BuecheRastriginFunction<T, U>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return static_cast<U>(100.0L) * this->getBoundConstraintsValue(parameter);
    }

    template <typename T, typename U>
    U BuecheRastriginFunction<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> z = parameterConditioning_ % this->getOscillatedParameter(parameter);
      for (std::size_t n = 0; n < z.n_elem; n += 2) {
        if (z(n) > static_cast<T>(0.0L)) {
          z(n) *= static_cast<T>(10.0L);
        }
      }

      return static_cast<U>(10.0L) * (static_cast<U>(this->numberOfDimensions_) - static_cast<U>(arma::accu(arma::cos(static_cast<T>(2.0L) * arma::datum::pi * z)))) + std::pow(static_cast<U>(arma::norm(z)), static_cast<U>(2.0L));
    }

    template <typename T, typename U>
    std::string BuecheRastriginFunction<T, U>::toString() const noexcept {
      return "bbob_bueche_rastrigin_function";
    }
    
#if defined(MANTELLA_USE_MPI)
    template <typename T, typename U>
    std::vector<double> BuecheRastriginFunction<T, U>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T, T>::serialise();
    }

    template <typename T, typename U>
    void BuecheRastriginFunction<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
