namespace mant {
  namespace bbob {
    class EllipsoidalFunctionRotated : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit EllipsoidalFunctionRotated(
            const unsigned int numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditoning_;

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
            cereal::construct<EllipsoidalFunctionRotated>& construct) noexcept {
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

    inline EllipsoidalFunctionRotated::EllipsoidalFunctionRotated(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        parameterConditoning_(getParameterConditioning(1000000.0)) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline double EllipsoidalFunctionRotated::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return arma::dot(parameterConditoning_, arma::square(getOscillatedParameter(parameter)));
    }

    inline std::string EllipsoidalFunctionRotated::toString() const noexcept {
      return "bbob_ellipsoidal_function_rotated";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::EllipsoidalFunctionRotated);
#endif
