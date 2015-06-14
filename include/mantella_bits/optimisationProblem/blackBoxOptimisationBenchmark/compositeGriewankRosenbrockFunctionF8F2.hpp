namespace mant {
  namespace bbob {
    template <typename T = double>
    class CompositeGriewankRosenbrockFunctionF8F2 : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
      public:
        explicit CompositeGriewankRosenbrockFunctionF8F2(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const double max_;

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
    CompositeGriewankRosenbrockFunctionF8F2<T>::CompositeGriewankRosenbrockFunctionF8F2(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        max_(std::max(1.0, std::sqrt(static_cast<double>(this->numberOfDimensions_)) / 8.0)){
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    double CompositeGriewankRosenbrockFunctionF8F2<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
       const arma::Col<T>& s = max_ * parameter + static_cast<T>(0.5L);
      const arma::Col<T>& z = static_cast<T>(100.0L) * arma::square(arma::square(s.head(s.n_elem - 1)) - s.tail(s.n_elem - 1)) + arma::square(s.head(s.n_elem - 1) - static_cast<T>(1.0L));

      return 10.0 * (static_cast<double>(arma::mean(z / static_cast<T>(4000.0L) - arma::cos(z))) + 1.0);
    }

    template <typename T>
    std::string CompositeGriewankRosenbrockFunctionF8F2<T>::toString() const noexcept {
      return "bbob_composite_griewank_rosenbrock_function_f8f2";
    }
    
#if defined(MANTELLA_USE_MPI)
    template <typename T>
    std::vector<double> CompositeGriewankRosenbrockFunctionF8F2<T>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T, T>::serialise();
    }

    template <typename T>
    void CompositeGriewankRosenbrockFunctionF8F2<T>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}