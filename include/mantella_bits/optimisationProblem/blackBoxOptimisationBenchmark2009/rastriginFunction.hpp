namespace mant {
  namespace bbob2009 {
    class RastriginFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit RastriginFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setLocalParameterTranslation(
            const arma::Col<double>& localParameterTranslation);

        inline std::string toString() const noexcept override;

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
          archive(cereal::make_nvp("translation", translation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<RastriginFunction>& construct) noexcept {
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

    inline RastriginFunction::RastriginFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        scaling_(getScaledTransformation(std::sqrt(10.0))) {
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
    }

    inline void RastriginFunction::setLocalParameterTranslation(
        const arma::Col<double>& localParameterTranslation) {
      checkDimensionCompatible("The number of elements", localParameterTranslation.n_elem, "the number of dimensions", numberOfDimensions_);

      localParameterTranslation_ = localParameterTranslation;
    }

    inline double RastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = scaling_ % getAsymmetricTransformation(0.2, getOscillatedTransformation(parameter - localParameterTranslation_));

      return 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + std::pow(arma::norm(z), 2.0);
    }

    inline std::string RastriginFunction::toString() const noexcept {
      return "rastrigin-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::RastriginFunction);
#endif
