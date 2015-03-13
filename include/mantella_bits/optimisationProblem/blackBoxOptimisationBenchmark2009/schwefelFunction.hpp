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
        const arma::Col<double> parameterConditioning_;

        arma::Col<double> parameterReflection_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
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

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("parameterReflection", construct->parameterReflection_));
        }
#endif
    };

    //
    // Implementation
    //

    inline SchwefelFunction::SchwefelFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      setParameterScaling(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 2.0 : -2.0));
      setParameterReflection(std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? true : false);
    }

    inline void SchwefelFunction::setParameterReflection(
        const bool parameterReflection) noexcept {
      parameterReflection_ = arma::zeros<arma::Col<double>>(numberOfDimensions_) + (parameterReflection ? 2.10484373165 : -2.10484373165);
    }

    inline double SchwefelFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> s = parameter;
      s.tail(s.n_elem - 1) += 0.25 * (parameter.head(parameter.n_elem - 1) - parameterReflection_.head(parameterReflection_.n_elem - 1));

      const arma::Col<double>& z = 100.0 * (parameterConditioning_ % (s - parameterReflection_) + parameterReflection_);

      return 0.01 * (418.9828872724339 - arma::mean(z % arma::sin(arma::sqrt(arma::abs(z))))) + 100.0 * getBoundConstraintsValue(z / 100.0);
    }

    inline std::string SchwefelFunction::toString() const noexcept {
      return "schwefel-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob2009::SchwefelFunction);
#endif
