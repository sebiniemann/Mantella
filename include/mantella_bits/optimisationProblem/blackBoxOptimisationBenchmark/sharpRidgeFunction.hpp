namespace mant {
  namespace bbob {
    class SharpRidgeFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit SharpRidgeFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline void setParameterRotationR(
            const arma::Mat<double>& parameterRotationR);

        inline void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditioning_;

        arma::Mat<double> parameterRotationR_;
        arma::Mat<double> rotationQ_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("parameterRotationR", parameterRotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<SharpRidgeFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
          archive(cereal::make_nvp("parameterRotationR", construct->parameterRotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline SharpRidgeFunction::SharpRidgeFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotationR(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void SharpRidgeFunction::setParameterRotationR(
        const arma::Mat<double>& parameterRotationR) {
      isEqual("The number of rows", parameterRotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      isRotationMatrix("The matrix", parameterRotationR);

      parameterRotationR_ = parameterRotationR;
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void SharpRidgeFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      isEqual("The number of rows", rotationQ.n_rows, "the number of dimensions", numberOfDimensions_);
      isRotationMatrix("The matrix", rotationQ);

      rotationQ_ = rotationQ;
    }

    inline double SharpRidgeFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = parameterRotationQ_ * (parameterConditioning_ % (parameterRotationR_ * parameter));
      return std::pow(z(0), 2.0) + 100.0 * arma::norm(z.tail(z.n_elem - 1));
    }

    inline std::string SharpRidgeFunction::toString() const noexcept {
      return "bbob_sharp_ridge_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::SharpRidgeFunction);
#endif
