namespace mant {
  namespace bbob2009 {
    class BuecheRastriginFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit BuecheRastriginFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

        inline void setLocalParameterTranslation(
            const arma::Col<double>& localParameterTranslation);

      protected:
        const arma::Col<double> scaling_;

        arma::Col<double> localParameterTranslation_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;
        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("localParameterTranslation", localParameterTranslation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<BuecheRastriginFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("localParameterTranslation", construct->localParameterTranslation_));
        }
#endif
    };

    //
    // Implementation
    //

    inline BuecheRastriginFunction::BuecheRastriginFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        scaling_(getScaledTransformation(std::sqrt(10.0))) {
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
    }

    inline void BuecheRastriginFunction::setLocalParameterTranslation(
        const arma::Col<double>& localParameterTranslation) {
      if (localParameterTranslation.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the local translation (" + std::to_string(localParameterTranslation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      localParameterTranslation_ = localParameterTranslation;
      for (std::size_t n = 0; n < localParameterTranslation.n_elem; n += 2) {
        localParameterTranslation_(n) = std::abs(localParameterTranslation(n));
      }
    }

    inline double BuecheRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = scaling_ % getOscillatedTransformation(parameter - localParameterTranslation_);
      for (std::size_t n = 0; n < z.n_elem; n += 2) {
        if (z(n) > 0.0) {
          z(n) *= 10.0;
        }
      }

      return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0) + 100.0 * getPenality(parameter);
    }

    inline std::string BuecheRastriginFunction::toString() const noexcept {
      return "bueche-rastrigin-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::BuecheRastriginFunction);
#endif
