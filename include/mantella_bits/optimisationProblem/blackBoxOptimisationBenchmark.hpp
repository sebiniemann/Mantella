namespace mant {
  namespace bbob {
    template <typename T = double>
    class BlackBoxOptimisationBenchmark : public OptimisationProblem<T, T> {
    static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
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

        T getBoundConstraintsValue(
            const arma::Col<T>& parameter) const noexcept;

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
        friend class OptimisationAlgorithm;
            
        std::vector<double> serialise() const noexcept;

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
      this->setLowerBounds(arma::zeros<arma::Col<T>>(this->numberOfDimensions_) - 5.0);
      this->setUpperBounds(arma::zeros<arma::Col<T>>(this->numberOfDimensions_) + 5.0);
      this->setObjectiveValueTranslation(std::min(1000.0, std::max(-1000.0, arma::floor(std::cauchy_distribution<double>(0.0, 100.0)(Rng::getGenerator()) * 100) / 100)));
      this->setAcceptableObjectiveValue(this->objectiveValueTranslation_ + 1.0e-8);
    }

    template <typename T>
    arma::Col<T> BlackBoxOptimisationBenchmark<T>::getRandomParameterTranslation() const noexcept {
      arma::Col<T> randomParameterTranslation = arma::floor(arma::randu<arma::Col<T>>(this->numberOfDimensions_) * 1.0e4) / 1.0e4 * 8.0 - 4.0;
      randomParameterTranslation.elem(arma::find(randomParameterTranslation == 0)).fill(-1.0e-5);
      return randomParameterTranslation;
    }

    template <typename T>
    arma::Col<T> BlackBoxOptimisationBenchmark<T>::getParameterConditioning(
        const T conditionNumber) const noexcept {
      arma::Col<T> parameterConditioning = arma::linspace<arma::Col<T>>(0.0, 1.0, this->numberOfDimensions_);

      for (T& conditioning : parameterConditioning) {
        conditioning = std::pow(conditionNumber, conditioning);
      }

      return parameterConditioning;
    }

    template <typename T>
    arma::Col<T> BlackBoxOptimisationBenchmark<T>::getConditionedParameter(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> conditionedParameter = arma::linspace<arma::Col<T>>(0.0, 1.0, this->numberOfDimensions_);

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
      const arma::Col<T>& spacing = arma::linspace<arma::Col<T>>(0.0, 1.0, this->numberOfDimensions_);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        const T value = parameter(n);

        if (value > 0.0) {
          asymmetricParameter(n) = std::pow(value, 1 + asymmetry * spacing(n) * std::sqrt(value));
        } else {
          asymmetricParameter(n) = value;
        }
      }

      return asymmetricParameter;
    }

    template <typename T>
    T BlackBoxOptimisationBenchmark<T>::getOscillatedValue(
        const T value) const noexcept {
      if (value != 0.0) {
        T c1;
        T c2;
        if (value > 0.0) {
          c1 = 10.0;
          c2 = 7.9;
        } else {
          c1 = 5.5;
          c2 = 3.1;
        }

        const T& logAbsoluteValue = std::log(std::abs(value));
        return std::copysign(1.0, value) * std::exp(logAbsoluteValue + 0.049 * (std::sin(c1 * logAbsoluteValue) + std::sin(c2 * logAbsoluteValue)));
      } else {
        return 0.0;
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
    T BlackBoxOptimisationBenchmark<T>::getBoundConstraintsValue(
        const arma::Col<T>& parameter) const noexcept {
      T boundConstraintsValue = 0.0;

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        boundConstraintsValue += std::pow(std::max(0.0, std::abs(parameter(n)) - 5.0), 2.0);
      }

      return boundConstraintsValue;
    }

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T>
    std::vector<double> BlackBoxOptimisationBenchmark<T>::serialise() const noexcept {
      return OptimisationProblem<T, T>::serialise();
    }

    template <typename T>
    void BlackBoxOptimisationBenchmark<T>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      OptimisationProblem<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
