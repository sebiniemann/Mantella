namespace mant {
  namespace bbob2009 {
    class EllipsoidalFunctionRotated : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit EllipsoidalFunctionRotated(
            const unsigned int numberOfDimensions) noexcept;

        inline void setParameterRotationR(
            const arma::Mat<double>& parameterRotationR);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditoning_;

        arma::Mat<double> parameterRotationR_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("parameterRotationR", parameterRotationR_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<EllipsoidalFunctionRotated>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("parameterRotationR", construct->parameterRotationR_));
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
      setParameterRotationR(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void EllipsoidalFunctionRotated::setParameterRotationR(
        const arma::Mat<double>& parameterRotationR) {
      checkCompatibleDimension("The number of rows", parameterRotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", parameterRotationR);

      parameterRotationR_ = parameterRotationR;
    }

    inline double EllipsoidalFunctionRotated::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return arma::dot(parameterConditoning_, arma::square(getOscillatedParameter(parameterRotationR_ * parameter)));
    }

    inline std::string EllipsoidalFunctionRotated::toString() const noexcept {
      return "ellipsoidal-function-rotated";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob2009::EllipsoidalFunctionRotated);
#endif
