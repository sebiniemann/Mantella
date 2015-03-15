namespace mant {
  namespace bbob2009 {
    class BlackBoxOptimisationBenchmark2009 : public OptimisationProblem<double> {
      public:
        inline explicit BlackBoxOptimisationBenchmark2009(
            const unsigned int& numberOfDimensions) noexcept;

        virtual ~BlackBoxOptimisationBenchmark2009() = default;

      protected:
        inline arma::Col<double> getRandomParameterTranslation() const noexcept;

        inline arma::Col<double> getParameterConditioning(
            const double& conditionNumber) const noexcept;

        inline arma::Col<double> getConditionedParameter(
            const arma::Col<double>& parameter) const noexcept;

        inline arma::Col<double> getAsymmetricParameter(
            const double& asymmetry, const arma::Col<double>& parameter) const noexcept;

        inline double getOscillatedValue(
            const double& oscilliation) const noexcept;

        inline arma::Col<double> getOscillatedParameter(
            const arma::Col<double>& parameter) const noexcept;

        inline double getBoundConstraintsValue(
            const arma::Col<double>& parameter) const noexcept;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;
        BlackBoxOptimisationBenchmark2009() = default;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("optimisationProblem", cereal::base_class<OptimisationProblem>(this)));
        }
#endif
    };

    //
    // Implementation
    //

    inline BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009(
        const unsigned int& numberOfDimensions) noexcept
      : OptimisationProblem(numberOfDimensions) {
      setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - 5.0);
      setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + 5.0);
      setObjectiveValueTranslation(std::min(1000.0, std::max(-1000.0, std::cauchy_distribution<double>(0.0, 100.0)(Rng::getGenerator()))));
      setAcceptableObjectiveValue(objectiveValueTranslation_ + 1.0e-8);
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getRandomParameterTranslation() const noexcept {
      arma::Col<double> randomParameterTranslation = arma::floor(arma::randu<arma::Col<double>>(numberOfDimensions_) * 1.0e4) / 1.0e4 * 8.0 - 4.0;
      randomParameterTranslation.elem(arma::find(randomParameterTranslation == 0)).fill(-1.0e5);
      return randomParameterTranslation;
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getParameterConditioning(
        const double& conditionNumber) const noexcept {
      arma::Col<double> parameterConditioning = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (auto& conditioning : parameterConditioning) {
        conditioning = std::pow(conditionNumber, conditioning);
      }

      return parameterConditioning;
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getConditionedParameter(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> conditionedParameter = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        conditionedParameter(n) = std::pow(parameter(n), conditionedParameter(n));
      }

      return conditionedParameter;
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getAsymmetricParameter(
        const double& asymmetry,
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> asymmetricParameter(parameter.n_elem);
      const arma::Col<double>& spacing = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        const double& value = parameter(n);

        if (value > 0.0) {
          asymmetricParameter(n) = std::pow(value, 1 + asymmetry * spacing(n) * std::sqrt(value));
        } else {
          asymmetricParameter(n) = value;
        }
      }

      return asymmetricParameter;
    }

    inline double BlackBoxOptimisationBenchmark2009::getOscillatedValue(
        const double& value) const noexcept {
      if (value != 0.0) {
        double c1;
        double c2;
        if (value > 0.0) {
          c1 = 10.0;
          c2 = 7.9;
        } else {
          c1 = 5.5;
          c2 = 3.1;
        }

        const double& logAbsoluteValue = std::log(std::abs(value));
        return std::copysign(1.0, value) * std::exp(logAbsoluteValue + 0.049 * (std::sin(c1 * logAbsoluteValue) + std::sin(c2 * logAbsoluteValue)));
      } else {
        return 0.0;
      }
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getOscillatedParameter(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> oscillatedParameter(parameter.n_elem);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        oscillatedParameter(n) = getOscillatedValue(parameter(n));
      }

      return oscillatedParameter;
    }

    inline double BlackBoxOptimisationBenchmark2009::getBoundConstraintsValue(
        const arma::Col<double>& parameter) const noexcept {
      double boundConstraintsValue = 0.0;

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        boundConstraintsValue += std::pow(std::max(0.0, std::abs(parameter(n)) - 5.0), 2.0);
      }

      return boundConstraintsValue;
    }
  }
}
