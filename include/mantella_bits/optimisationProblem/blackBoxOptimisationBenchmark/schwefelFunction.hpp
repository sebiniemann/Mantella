namespace mant {
  namespace bbob {
    class SchwefelFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit SchwefelFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditioning_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("parameterReflection", parameterReflection_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<SchwefelFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
          archive(cereal::make_nvp("parameterReflection", construct->parameterReflection_));
        }
#endif
    };

    //
    // Implementation
    //

    inline SchwefelFunction::SchwefelFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      setParameterScaling(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 2.0 : -2.0));
    }

    inline double SchwefelFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> s = parameter;
      s.tail(s.n_elem - 1) += 0.25 * (s.head(s.n_elem - 1) - 4.2096874633);

      const arma::Col<double>& z = 100.0 * (parameterConditioning_ % (s - 4.2096874633) + 4.2096874633);

      return 0.01 * (418.9828872724339 - arma::dot(z, arma::sin(arma::sqrt(arma::abs(z)))) / static_cast<double>(numberOfDimensions_)) + 100.0 * getBoundConstraintsValue(z / 100.0);
    }

    inline std::string SchwefelFunction::toString() const noexcept {
      return "bbob_schwefel_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::SchwefelFunction);
#endif
