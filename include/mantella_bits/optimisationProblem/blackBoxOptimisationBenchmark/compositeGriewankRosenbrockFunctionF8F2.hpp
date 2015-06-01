namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class CompositeGriewankRosenbrockFunctionF8F2 : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit CompositeGriewankRosenbrockFunctionF8F2(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const T max_;

        T getObjectiveValueImplementation(
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
    CompositeGriewankRosenbrockFunctionF8F2<T, U>::CompositeGriewankRosenbrockFunctionF8F2(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        max_(std::max(static_cast<T>(1.0L), std::sqrt(static_cast<T>(this->numberOfDimensions_)) / static_cast<T>(8.0L))){
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T, typename U>
    T CompositeGriewankRosenbrockFunctionF8F2<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
       const arma::Col<T>& s = max_ * parameter + static_cast<T>(0.5L);
      const arma::Col<T>& z = static_cast<T>(100.0L) * arma::square(arma::square(s.head(s.n_elem - 1)) - s.tail(s.n_elem - 1)) + arma::square(s.head(s.n_elem - 1) - static_cast<T>(1.0L));

      return static_cast<T>(10.0L) * (arma::mean(z / static_cast<T>(4000.0L) - arma::cos(z)) + static_cast<T>(1L));
    }

    template <typename T, typename U>
    std::string CompositeGriewankRosenbrockFunctionF8F2<T, U>::toString() const noexcept {
      return "bbob_composite_griewank_rosenbrock_function_f8f2";
    }
    
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> CompositeGriewankRosenbrockFunctionF8F2<T, U>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T, T>::serialise();
    }

    template <typename T, typename U>
    void CompositeGriewankRosenbrockFunctionF8F2<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}