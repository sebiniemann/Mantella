namespace mant {
  namespace bbob {
    template <typename T = double>
    class SchwefelFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
      public:
        explicit SchwefelFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

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
    SchwefelFunction<T>::SchwefelFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      this->setParameterScaling(arma::zeros<arma::Col<T>>(this->numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? static_cast<T>(2.0L) : static_cast<T>(-2.0L)));
    }

    template <typename T>
    double SchwefelFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> s = parameter;
      s.tail(s.n_elem - 1) += static_cast<T>(0.25L) * (s.head(s.n_elem - 1) - static_cast<T>(4.2096874633L));

      const arma::Col<T>& z = static_cast<T>(100.0L) * (parameterConditioning_ % (s - static_cast<T>(4.2096874633L)) + static_cast<T>(4.2096874633L));

      return 0.01 * (418.9828872724339 - static_cast<double>(arma::dot(z, arma::sin(arma::sqrt(arma::abs(z))))) / static_cast<double>(this->numberOfDimensions_)) + 100.0 * this->getBoundConstraintsValue(z / static_cast<T>(100.0L));
    }

    template <typename T>
    std::string SchwefelFunction<T>::toString() const noexcept {
      return "bbob_schwefel_function";
    }
    
#if defined(MANTELLA_USE_MPI)
    template <typename T>
    std::vector<double> SchwefelFunction<T>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T, T>::serialise();
    }

    template <typename T>
    void SchwefelFunction<T>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
