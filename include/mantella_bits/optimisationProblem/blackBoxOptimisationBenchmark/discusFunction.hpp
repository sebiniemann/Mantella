namespace mant {
  namespace bbob {
    template <typename T = double>
    class DiscusFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
      public:
        explicit DiscusFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        std::vector<double> serialise() const noexcept;

        // The type is intentionally fixed to ease usage with MPI_DOUBLE.
        void deserialise(
            std::vector<double> serialisedOptimisationProblem);

      protected:
        double getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;
    };

    //
    // Implementation
    //

    template <typename T>
    DiscusFunction<T>::DiscusFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    double DiscusFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = arma::square(this->getOscillatedParameter(parameter));
      return 1000000.0 * static_cast<double>(z(0)) + static_cast<double>(arma::accu(z.tail(z.n_elem - 1)));
    }

    template <typename T>
    std::string DiscusFunction<T>::toString() const noexcept {
      return "bbob_discus_function";
    }
    
#if defined(MANTELLA_USE_MPI)
    template <typename T>
    std::vector<double> DiscusFunction<T>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T>::serialise();
    }

    template <typename T>
    void DiscusFunction<T>::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}