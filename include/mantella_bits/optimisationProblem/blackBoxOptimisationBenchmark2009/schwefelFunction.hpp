namespace mant {
  namespace bbob2009 {
    class SchwefelFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit SchwefelFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setParameterReflection(
            const bool Parameterreflection) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> scaling_;

        arma::Col<double> Parameterreflection_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("refletion", refletion_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<SchwefelFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("refletion", construct->refletion_));
        }
#endif
    };

    //
    // Implementation
    //

    inline SchwefelFunction::SchwefelFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        scaling_(getScaledTransformation(std::sqrt(10.0))) {
      setParameterReflection(std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? true : false);
    }

    inline void SchwefelFunction::setParameterReflection(
        const bool Parameterreflection) noexcept {
      if (Parameterreflection) {
        Parameterreflection_ = -arma::ones<arma::Col<double>>(numberOfDimensions_);
      } else {
        Parameterreflection_ = arma::ones<arma::Col<double>>(numberOfDimensions_);
      }
    }

    inline double SchwefelFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& localParameterTranslation = 2.10484373165 * Parameterreflection_;
      const arma::Col<double>& xHat = 2.0 * Parameterreflection_ % parameter;

      arma::Col<double> zHat(xHat.n_elem);
      zHat(0) = xHat(0);
      zHat.tail(zHat.n_elem - 1) = xHat.tail(zHat.n_elem - 1) + 0.25 * (xHat.head(xHat.n_elem - 1) - localParameterTranslation.head(localParameterTranslation.n_elem - 1));

      const arma::Col<double>& z = 100.0 * (scaling_ % (zHat - localParameterTranslation) + localParameterTranslation);

      return 0.01 * (418.9828872724339 - arma::mean(z % arma::sin(arma::sqrt(arma::abs(z))))) + 100.0 * getPenality(z / 100.0);
    }

    inline std::string SchwefelFunction::toString() const noexcept {
      return "schwefel-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::SchwefelFunction);
#endif
