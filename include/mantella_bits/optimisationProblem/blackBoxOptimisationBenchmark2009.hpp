namespace mant {
  namespace bbob2009 {
    class BlackBoxOptimisationBenchmark2009 : public OptimisationProblem<double> {
      public:
        inline explicit BlackBoxOptimisationBenchmark2009(
            const unsigned int& numberOfDimensions) noexcept;

        virtual ~BlackBoxOptimisationBenchmark2009() = default;

      protected:
        inline arma::Col<double> getRandomLocalParameterTranslation() const noexcept;

        inline arma::Col<double> getScaledTransformation(
            const double& condition) const noexcept;

        inline arma::Col<double> getScaledTransformation(
            const arma::Col<double>& condition) const noexcept;

        inline arma::Col<double> getAsymmetricTransformation(
            const double& beta, const arma::Col<double>& parameter) const noexcept;

        inline double getOscillatedTransformation(
            const double& parameter) const noexcept;

        inline arma::Col<double> getOscillatedTransformation(
            const arma::Col<double>& parameter) const noexcept;

        inline double getPenality(
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

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getRandomLocalParameterTranslation() const noexcept {
      arma::Col<double> localParameterTranslation = arma::floor(arma::randu<arma::Col<double>>(numberOfDimensions_) * 1.0e4) / 1.0e4 * 8.0 - 4.0;
      localParameterTranslation.elem(arma::find(localParameterTranslation == 0)).fill(-1.0e5);
      return localParameterTranslation;
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getScaledTransformation(
        const double& condition) const noexcept {
      arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (auto& scale : scaling) {
        scale = std::pow(condition, scale);
      }

      return scaling;
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getScaledTransformation(
        const arma::Col<double>& condition) const noexcept {
      arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (std::size_t n = 0; n < scaling.n_elem; ++n) {
        scaling(n) = std::pow(condition(n), scaling(n));
      }

      return scaling;
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getAsymmetricTransformation(
        const double& beta,
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> asymmetricTransformation(parameter.n_elem);
      const arma::Col<double>& spacing = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        const double& value = parameter(n);

        if (value > 0.0) {
          asymmetricTransformation(n) = std::pow(value, 1 + beta * spacing(n) * std::sqrt(value));
        } else {
          asymmetricTransformation(n) = value;
        }
      }

      return asymmetricTransformation;
    }

    inline double BlackBoxOptimisationBenchmark2009::getOscillatedTransformation(
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

        const double& x = std::log(std::abs(value));
        return std::copysign(1.0, value) * std::exp(x + 0.049 * (std::sin(c1 * x) + std::sin(c2 * x)));
      } else {
        return 0.0;
      }
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getOscillatedTransformation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> oscillated(parameter.n_elem);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        oscillated(n) = getOscillatedTransformation(parameter(n));
      }

      return oscillated;
    }

    inline double BlackBoxOptimisationBenchmark2009::getPenality(
        const arma::Col<double>& parameter) const noexcept {
      double penality = 0.0;

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        penality += std::pow(std::max(0.0, std::abs(parameter(n)) - 5.0), 2.0);
      }

      return penality;
    }
  }
}
