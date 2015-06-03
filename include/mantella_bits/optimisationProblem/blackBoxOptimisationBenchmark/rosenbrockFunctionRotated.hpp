namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class RosenbrockFunctionRotated : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit RosenbrockFunctionRotated(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const U max_;

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
    RosenbrockFunctionRotated<T, U>::RosenbrockFunctionRotated(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        max_(std::max(static_cast<U>(1.0L), std::sqrt(static_cast<U>(this->numberOfDimensions_)) / static_cast<U>(8.0L))) {
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T, typename U>
    U RosenbrockFunctionRotated<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = max_ * parameter + static_cast<T>(0.5L);

      return static_cast<U>(100.0L) * std::pow(static_cast<U>(arma::norm(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1))), static_cast<U>(2.0L)) + std::pow(static_cast<U>(arma::norm(z.head(z.n_elem - 1) - static_cast<T>(1.0L))), static_cast<U>(2.0L));
    }

    template <typename T, typename U>
    std::string RosenbrockFunctionRotated<T, U>::toString() const noexcept {
      return "bbob_rosenbrock_function_rotated";
    }
    
#if defined(MANTELLA_USE_MPI)
    template <typename T, typename U>
    std::vector<double> RosenbrockFunctionRotated<T, U>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T, T>::serialise();
    }

    template <typename T, typename U>
    void RosenbrockFunctionRotated<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
