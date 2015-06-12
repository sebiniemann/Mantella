namespace mant {
  namespace bbob {
    template <typename T = double>
    class BuecheRastriginFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
      public:
        explicit BuecheRastriginFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

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
    BuecheRastriginFunction<T>::BuecheRastriginFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      arma::Col<T> parameterTranslation = this->getRandomParameterTranslation();
      for (std::size_t n = 0; n < parameterTranslation.n_elem; n += 2) {
        parameterTranslation(n) = std::abs(parameterTranslation(n));
      }
      this->setParameterTranslation(parameterTranslation);
    }

    template <typename T>
    double BuecheRastriginFunction<T>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return 100.0 * this->getBoundConstraintsValue(parameter);
    }

    template <typename T>
    double BuecheRastriginFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> z = parameterConditioning_ % this->getOscillatedParameter(parameter);
      for (std::size_t n = 0; n < z.n_elem; n += 2) {
        if (z(n) > 0.0) {
          z(n) *= 10.0;
        }
      }

      return 10.0 * (static_cast<double>(this->numberOfDimensions_) - static_cast<double>(arma::accu(arma::cos(static_cast<T>(2.0L) * arma::datum::pi * z)))) + std::pow(static_cast<double>(arma::norm(z)), 2.0);
    }

    template <typename T>
    std::string BuecheRastriginFunction<T>::toString() const noexcept {
      return "bbob_bueche_rastrigin_function";
    }
    
#if defined(MANTELLA_USE_MPI)
    template <typename T>
    std::vector<double> BuecheRastriginFunction<T>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T, T>::serialise();
    }

    template <typename T>
    void BuecheRastriginFunction<T>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
