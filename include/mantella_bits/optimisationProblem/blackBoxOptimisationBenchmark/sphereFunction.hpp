namespace mant {
  namespace bbob {
    class SphereFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit SphereFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

      protected:
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
            cereal::construct<SphereFunction>& construct) noexcept {
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

    inline SphereFunction::SphereFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions) {
      setParameterTranslation(getRandomParameterTranslation());
    }

    inline double SphereFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return std::pow(arma::norm(parameter), 2.0);
    }

    inline std::string SphereFunction::toString() const noexcept {
      return "bbob_sphere_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::SphereFunction);
#endif
