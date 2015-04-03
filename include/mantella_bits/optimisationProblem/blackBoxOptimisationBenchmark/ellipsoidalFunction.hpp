namespace mant {
  namespace bbob {
    class EllipsoidalFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit EllipsoidalFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditioning_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<EllipsoidalFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
        }
#endif
    };

    //
    // Implementation
    //

    inline EllipsoidalFunction::EllipsoidalFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(1000000.0)) {

    }

    inline double EllipsoidalFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return arma::dot(parameterConditioning_, arma::square(getOscillatedParameter(parameter)));
    }

    inline std::string EllipsoidalFunction::toString() const noexcept {
      return "bbob_ellipsoidal_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::EllipsoidalFunction);
#endif
