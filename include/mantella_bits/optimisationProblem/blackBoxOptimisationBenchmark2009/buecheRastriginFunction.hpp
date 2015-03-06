namespace mant {
  namespace bbob2009 {
    class BuecheRastriginFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit BuecheRastriginFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

        inline void setTranslation(
            const arma::Col<double> translation) override;

      protected:
        const arma::Col<double> scaling_ = getScaling(std::sqrt(10.0));

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;
        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<BuecheRastriginFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
        }
#endif
    };

    //
    // Implementation
    //

    inline BuecheRastriginFunction::BuecheRastriginFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      setTranslation(translation_);
    }

    inline void BuecheRastriginFunction::setTranslation(
        const arma::Col<double> translation) {
      if (translation.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the translation (" + std::to_string(translation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      translation_ = translation;
      for (std::size_t n = 0; n < translation_.n_elem; n += 2) {
        translation_(n) = std::abs(translation_(n));
      }
    }

    inline double BuecheRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = scaling_ % getOscillationTransformation(parameter - translation_);
      for (std::size_t n = 0; n < z.n_elem; n += 2) {
        if (z(n) > 0.0) {
          z(n) *= 10.0;
        }
      }

      return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0) + 100.0 * getPenality(parameter);
    }

    inline std::string BuecheRastriginFunction::toString() const noexcept {
      return "BuecheRastriginFunction";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::BuecheRastriginFunction);
#endif
