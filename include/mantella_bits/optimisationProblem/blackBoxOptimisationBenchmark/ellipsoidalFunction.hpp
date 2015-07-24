namespace mant {
  namespace bbob {
    template <typename T = double>
    class EllipsoidalFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
      public:
        explicit EllipsoidalFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        std::vector<double> serialise() const noexcept;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        void deserialise(
            std::vector<double> serialisedOptimisationProblem);

      protected:
        const arma::Col<T> parameterConditioning_;

        double getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;
    };

    //
    // Implementation
    //

    template <typename T>
    EllipsoidalFunction<T>::EllipsoidalFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(static_cast<T>(1000000.0L))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
    }

    template <typename T>
    double EllipsoidalFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return static_cast<double>(arma::dot(parameterConditioning_, arma::square(this->getOscillatedParameter(parameter))));
    }

    template <typename T>
    std::string EllipsoidalFunction<T>::toString() const noexcept {
      return "bbob_ellipsoidal_function";
    }
    
#if defined(MANTELLA_USE_MPI)
    template <typename T>
    std::vector<double> EllipsoidalFunction<T>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T>::serialise();
    }

    template <typename T>
    void EllipsoidalFunction<T>::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
