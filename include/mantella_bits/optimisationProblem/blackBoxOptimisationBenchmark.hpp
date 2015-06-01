namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class BlackBoxOptimisationBenchmark : public OptimisationProblem<T, U> {
    static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
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

        U getOscillatedValue(
            const U oscilliation) const noexcept;

        T getOscillatedValue(
            const T oscilliation) const noexcept;

        arma::Col<T> getOscillatedParameter(
            const arma::Col<T>& parameter) const noexcept;

        U getBoundConstraintsValue(
            const arma::Col<T>& parameter) const noexcept;

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
    BlackBoxOptimisationBenchmark<T, U>::BlackBoxOptimisationBenchmark(
        const std::size_t numberOfDimensions) noexcept
      : OptimisationProblem<T, U>(numberOfDimensions) {
      // A vector with all elements set to -5.
      this->setLowerBounds(arma::zeros<arma::Col<T>>(this->numberOfDimensions_) - static_cast<T>(5.0L));
      // A vector with all elements set to 5.
      this->setUpperBounds(arma::zeros<arma::Col<T>>(this->numberOfDimensions_) + static_cast<T>(5.0L));
      // The objective value translation is randomly chosen from a Cauchy distribution with an approximate 50% chance to be within [-100, 100], rounded up to 2 decimal places.
      // The translation is further bounded between -1000 and 1000.
      this->setObjectiveValueTranslation(std::min(static_cast<U>(1000.0L), std::max(static_cast<U>(-1000.0L), std::floor(std::cauchy_distribution<U>(static_cast<U>(0.0L), static_cast<U>(100.0L))(Rng::getGenerator()) * static_cast<U>(100.0L)) / static_cast<U>(100.0L))));
      this->setAcceptableObjectiveValue(this->objectiveValueTranslation_ + static_cast<U>(1.0e-8L));
    }

    template <typename T, typename U>
    arma::Col<T> BlackBoxOptimisationBenchmark<T, U>::getRandomParameterTranslation() const noexcept {
      // The parameter space is randomly translated by [-4, 4]^N, rounded up to 4 decimal places.
      arma::Col<T> randomParameterTranslation = arma::floor(arma::randu<arma::Col<T>>(this->numberOfDimensions_) * static_cast<T>(1.0e4L)) / static_cast<T>(1.0e4L) * static_cast<T>(8.0L) - static_cast<T>(4.0L);
      // In case the parameter space would remain unchanged, it is forcefully translated by -0.00001.
      randomParameterTranslation.elem(arma::find(randomParameterTranslation == static_cast<T>(0.0L))).fill(static_cast<T>(-1.0e-5L));
      
      return randomParameterTranslation;
    }

    template <typename T, typename U>
    arma::Col<T> BlackBoxOptimisationBenchmark<T, U>::getParameterConditioning(
        const T conditionNumber) const noexcept {
      arma::Col<T> parameterConditioning = arma::linspace<arma::Col<T>>(static_cast<T>(0.0L), static_cast<T>(1.0L), this->numberOfDimensions_);

      // In-place calculation of the conditioning.
      for (T& conditioning : parameterConditioning) {
        conditioning = std::pow(conditionNumber, conditioning);
      }

      return parameterConditioning;
    }

    template <typename T, typename U>
    arma::Col<T> BlackBoxOptimisationBenchmark<T, U>::getConditionedParameter(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> conditionedParameter = arma::linspace<arma::Col<T>>(static_cast<T>(0.0L), static_cast<T>(1.0L), this->numberOfDimensions_);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        conditionedParameter(n) = std::pow(parameter(n), conditionedParameter(n));
      }

      return conditionedParameter;
    }

    template <typename T, typename U>
    arma::Col<T> BlackBoxOptimisationBenchmark<T, U>::getAsymmetricParameter(
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

    template <typename T, typename U>
    U BlackBoxOptimisationBenchmark<T, U>::getOscillatedValue(
        const U value) const noexcept {
      if (value != static_cast<U>(0.0L)) {
        T c1;
        T c2;
        if (value > static_cast<U>(0.0L)) {
          c1 = static_cast<U>(10.0L);
          c2 = static_cast<U>(7.9L);
        } else {
          c1 = static_cast<U>(5.5L);
          c2 = static_cast<U>(3.1L);
        }

        const T& logAbsoluteValue = std::log(std::abs(value));
        return std::copysign(static_cast<U>(1.0L), value) * std::exp(logAbsoluteValue + static_cast<U>(0.049L) * (std::sin(c1 * logAbsoluteValue) + std::sin(c2 * logAbsoluteValue)));
      } else {
        return static_cast<U>(0.0L);
      }
    }

    template <typename T, typename U>
    T BlackBoxOptimisationBenchmark<T, U>::getOscillatedValue(
        const T value) const noexcept {
      if (value != static_cast<U>(0.0L)) {
        T c1;
        T c2;
        if (value > static_cast<U>(0.0L)) {
          c1 = static_cast<U>(10.0L);
          c2 = static_cast<U>(7.9L);
        } else {
          c1 = static_cast<U>(5.5L);
          c2 = static_cast<U>(3.1L);
        }

        const T& logAbsoluteValue = std::log(std::abs(value));
        return std::copysign(static_cast<U>(1.0L), value) * std::exp(logAbsoluteValue + static_cast<U>(0.049L) * (std::sin(c1 * logAbsoluteValue) + std::sin(c2 * logAbsoluteValue)));
      } else {
        return static_cast<U>(0.0L);
      }
    }

    template <typename T, typename U>
    arma::Col<T> BlackBoxOptimisationBenchmark<T, U>::getOscillatedParameter(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> oscillatedParameter(parameter.n_elem);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        oscillatedParameter(n) = getOscillatedValue(parameter(n));
      }

      return oscillatedParameter;
    }

    template <typename T, typename U>
    U BlackBoxOptimisationBenchmark<T, U>::getBoundConstraintsValue(
        const arma::Col<T>& parameter) const noexcept {
      U boundConstraintsValue = static_cast<U>(0.0L);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        boundConstraintsValue += std::pow(std::max(static_cast<U>(0.0L), std::abs(parameter(n)) - static_cast<U>(5.0L)), static_cast<U>(2.0L));
      }

      return boundConstraintsValue;
    }

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> BlackBoxOptimisationBenchmark<T, U>::serialise() const noexcept {
      return OptimisationProblem<T, U>::serialise();
    }

    template <typename T, typename U>
    void BlackBoxOptimisationBenchmark<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      OptimisationProblem<T, U>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
