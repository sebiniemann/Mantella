namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class LinearSlope : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit LinearSlope(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;
        const U f0_;

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
    LinearSlope<T, U>::LinearSlope(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(static_cast<T>(10.0L))),
        f0_(static_cast<U>(5.0L) * static_cast<U>(arma::accu(parameterConditioning_))) {
      this->setParameterRotation(arma::eye<arma::Mat<T>>(this->numberOfDimensions_, this->numberOfDimensions_) * (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? static_cast<T>(1.0L) : static_cast<T>(-1.0L)));
    }

    template <typename T, typename U>
    U LinearSlope<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> z = parameter;
      z.elem(arma::find(parameter >= static_cast<T>(5.0L))).fill(static_cast<T>(5.0L));

      return f0_ - static_cast<U>(arma::dot(parameterConditioning_, z));
    }

    template <typename T, typename U>
    std::string LinearSlope<T, U>::toString() const noexcept {
      return "bbob_linear_slope";
    }
    
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> LinearSlope<T, U>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T, T>::serialise();
    }

    template <typename T, typename U>
    void LinearSlope<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
