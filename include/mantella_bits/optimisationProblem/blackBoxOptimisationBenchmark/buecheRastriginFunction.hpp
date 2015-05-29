namespace mant {
  namespace bbob {
    class BuecheRastriginFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit BuecheRastriginFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditioning_;

        inline double getSoftConstraintsValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;
        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<BuecheRastriginFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
        }
#endif
    };

    //
    // Implementation
    //

    inline BuecheRastriginFunction::BuecheRastriginFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      arma::Col<double> parameterTranslation_ = getRandomParameterTranslation();
      for (std::size_t n = 0; n < parameterTranslation_.n_elem; n += 2) {
        parameterTranslation_(n) = std::abs(parameterTranslation_(n));
      }
      setParameterTranslation(parameterTranslation_);
    }

    inline double BuecheRastriginFunction::getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return 100.0 * getBoundConstraintsValue(parameter);
    }

    inline double BuecheRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = parameterConditioning_ % getOscillatedParameter(parameter);
      for (std::size_t n = 0; n < z.n_elem; n += 2) {
        if (z(n) > 0.0) {
          z(n) *= 10.0;
        }
      }

      return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0);
    }

    inline std::string BuecheRastriginFunction::toString() const noexcept {
      return "bbob_bueche_rastrigin_function";
    }
  }
}
