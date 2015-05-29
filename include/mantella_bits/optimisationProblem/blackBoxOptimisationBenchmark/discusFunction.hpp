namespace mant {
  namespace bbob {
    template <typename T = double>
    class DiscusFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit DiscusFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        T getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
        friend class OptimisationAlgorithm;
        
        std::vector<long double> serialise() const noexcept;

        void deserialise(
            const std::vector<long double>& serialisedOptimisationProblem);
#endif
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
    T DiscusFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = arma::square(this->getOscillatedParameter(parameter));
      return static_cast<T>(1000000.0L) * z(0) + arma::accu(z.tail(z.n_elem - 1));
    }

    template <typename T>
    std::string DiscusFunction<T>::toString() const noexcept {
      return "bbob_discus_function";
    }
    
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T>
    std::vector<long double> BentCigarFunction<T>::serialise() const noexcept {
      return BlackBoxOptimisationBenchmark<T, T>::serialise();;
    }

    template <typename T>
    void BentCigarFunction<T>::deserialise(
        const std::vector<long double>& serialisedOptimisationProblem) {
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}