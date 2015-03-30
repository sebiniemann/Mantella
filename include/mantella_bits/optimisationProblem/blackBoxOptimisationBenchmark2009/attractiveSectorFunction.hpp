namespace mant {
  namespace bbob2009 {
    class AttractiveSectorFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit AttractiveSectorFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline void setParameterRotationQ(
            const arma::Mat<double>& parameterRotationQ);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditioning_;

        arma::Mat<double> parameterRotationQ_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("parameterRotationQ", parameterRotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<AttractiveSectorFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("parameterRotationQ", construct->parameterRotationQ_));
        }
#endif
    };
  
    //
    // Implementation
    //

    inline AttractiveSectorFunction::AttractiveSectorFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
      setParameterRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void AttractiveSectorFunction::setParameterRotationQ(
        const arma::Mat<double>& parameterRotationQ) {
      isEqual("The number of rows", parameterRotationQ.n_rows, "the number of dimensions", numberOfDimensions_);
      isRotationMatrix("The matrix", parameterRotationQ);

      parameterRotationQ_ = parameterRotationQ;
    }

    inline double AttractiveSectorFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = parameterRotationQ_ * (parameterConditioning_ % parameter);
      z.elem(arma::find(z % parameterTranslation_ > 0.0)) *= 100.0;

      return std::pow(getOscillatedValue(std::pow(arma::norm(z), 2.0)), 0.9);
    }

    inline std::string AttractiveSectorFunction::toString() const noexcept {
      return "bbob2009_attractive_sector_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob2009::AttractiveSectorFunction);
#endif
