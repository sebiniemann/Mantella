namespace mant {
  namespace bbob2009 {
    class DifferentPowersFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit DifferentPowersFunction(
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
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<DifferentPowersFunction>& construct) noexcept {
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

    inline DifferentPowersFunction::DifferentPowersFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline double DifferentPowersFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = arma::abs(parameter);
      return arma::norm(z % getConditionedParameter(arma::square(z)));
    }

    inline std::string DifferentPowersFunction::toString() const noexcept {
      return "different-powers-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob2009::DifferentPowersFunction);
#endif
