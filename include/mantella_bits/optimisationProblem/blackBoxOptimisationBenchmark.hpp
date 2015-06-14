namespace mant {
  namespace bbob {
    template <typename T = double>
    class BlackBoxOptimisationBenchmark : public OptimisationProblem<T> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
      public:
        explicit BlackBoxOptimisationBenchmark(
            const std::size_t numberOfDimensions) noexcept;

        virtual ~BlackBoxOptimisationBenchmark() = default;

      protected:
        arma::Col<T> getRandomParameterTranslation() const noexcept;

        arma::Col<T> getParameterConditioning(
            const T conditionNumber) const noexcept;

        arma::Col<T> getConditionedParameter(
            const arma::Col<T>& parameter) const noexcept;

        arma::Col<T> getAsymmetricParameter(
            const T asymmetry,
            const arma::Col<T>& parameter) const noexcept;
            
        T getOscillatedValue(
            const T oscilliation) const noexcept;

        arma::Col<T> getOscillatedParameter(
            const arma::Col<T>& parameter) const noexcept;

        double getBoundConstraintsValue(
            const arma::Col<T>& parameter) const noexcept;

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
    BlackBoxOptimisationBenchmark<T>::BlackBoxOptimisationBenchmark(
        const std::size_t numberOfDimensions) noexcept
      : OptimisationProblem<T>(numberOfDimensions) {
      // A vector with all elements set to -5.
      this->setLowerBounds(arma::zeros<arma::Col<T>>(this->numberOfDimensions_) - static_cast<T>(5.0L));
      // A vector with all elements set to 5.
      this->setUpperBounds(arma::zeros<arma::Col<T>>(this->numberOfDimensions_) + static_cast<T>(5.0L));
      // The objective value translation is randomly chosen from a Cauchy distribution with an approximate 50% chance to be within [-100, 100], rounded up to 2 decimal places.
      // The translation is further bounded between -1000 and 1000.
      this->setObjectiveValueTranslation(std::min(1000.0, std::max(-1000.0, std::floor(std::cauchy_distribution<double>(0.0, 100.0)(Rng::getGenerator()) * 100.0) / 100.0)));
      this->setAcceptableObjectiveValue(this->objectiveValueTranslation_ + 1.0e-8);
    }

    template <typename T>
    arma::Col<T> BlackBoxOptimisationBenchmark<T>::getRandomParameterTranslation() const noexcept {
      // The parameter space is randomly translated by [-4, 4]^N, rounded up to 4 decimal places.
      arma::Col<T> randomParameterTranslation = arma::floor(arma::randu<arma::Col<T>>(this->numberOfDimensions_) * static_cast<T>(1.0e4L)) / static_cast<T>(1.0e4L) * static_cast<T>(8.0L) - static_cast<T>(4.0L);
      // In case the parameter space would remain unchanged, it is forcefully translated by -0.00001.
      randomParameterTranslation.elem(arma::find(randomParameterTranslation == static_cast<T>(0.0L))).fill(static_cast<T>(-1.0e-5L));
      
      return randomParameterTranslation;
    }

    template <typename T>
    arma::Col<T> BlackBoxOptimisationBenchmark<T>::getParameterConditioning(
        const T conditionNumber) const noexcept {
      arma::Col<T> parameterConditioning = arma::linspace<arma::Col<T>>(static_cast<T>(0.0L), static_cast<T>(1.0L), this->numberOfDimensions_);

      // In-place calculation of the conditioning.
      for (T& conditioning : parameterConditioning) {
        conditioning = std::pow(conditionNumber, conditioning);
      }

      return parameterConditioning;
    }

    template <typename T>
    arma::Col<T> BlackBoxOptimisationBenchmark<T>::getConditionedParameter(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> conditionedParameter = arma::linspace<arma::Col<T>>(static_cast<T>(0.0L), static_cast<T>(1.0L), this->numberOfDimensions_);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        conditionedParameter(n) = std::pow(parameter(n), conditionedParameter(n));
      }

      return conditionedParameter;
    }

    template <typename T>
    arma::Col<T> BlackBoxOptimisationBenchmark<T>::getAsymmetricParameter(
        const T asymmetry,
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> asymmetricParameter(parameter.n_elem);
      const arma::Col<T>& spacing = arma::linspace<arma::Col<T>>(static_cast<T>(0.0L), static_cast<T>(1.0L), this->numberOfDimensions_);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        const T value = parameter(n);

        if (value > static_cast<T>(0.0L)) {
          asymmetricParameter(n) = std::pow(value, static_cast<T>(1.0L) + asymmetry * spacing(n) * std::sqrt(value));
        } else {
          asymmetricParameter(n) = value;
        }
      }

      return asymmetricParameter;
    }

    template <typename T>
    T BlackBoxOptimisationBenchmark<T>::getOscillatedValue(
        const T value) const noexcept {
      if (value != static_cast<T>(0.0L)) {
        T c1;
        T c2;
        if (value > static_cast<T>(0.0L)) {
          c1 = static_cast<T>(10.0L);
          c2 = static_cast<T>(7.9L);
        } else {
          c1 = static_cast<T>(5.5L);
          c2 = static_cast<T>(3.1L);
        }

        const T& logAbsoluteValue = std::log(std::abs(value));
        return std::copysign(static_cast<T>(1.0L), value) * std::exp(logAbsoluteValue + static_cast<T>(0.049L) * (std::sin(c1 * logAbsoluteValue) + std::sin(c2 * logAbsoluteValue)));
      } else {
        return static_cast<T>(0.0L);
      }
    }

    template <typename T>
    arma::Col<T> BlackBoxOptimisationBenchmark<T>::getOscillatedParameter(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> oscillatedParameter(parameter.n_elem);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        oscillatedParameter(n) = getOscillatedValue(parameter(n));
      }

      return oscillatedParameter;
    }

    template <typename T>
    double BlackBoxOptimisationBenchmark<T>::getBoundConstraintsValue(
        const arma::Col<T>& parameter) const noexcept {
      double boundConstraintsValue = 0.0;

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        boundConstraintsValue += std::pow(std::max(0.0, std::abs(static_cast<double>(parameter(n))) - 5.0), 2.0);
      }

      return boundConstraintsValue;
    }

#if defined(MANTELLA_USE_MPI)
    template <typename T>
    std::vector<double> BlackBoxOptimisationBenchmark<T>::serialise() const noexcept {
      return OptimisationProblem<T>::serialise();
    }

    template <typename T>
    void BlackBoxOptimisationBenchmark<T>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      OptimisationProblem<T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
