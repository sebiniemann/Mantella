namespace mant {
  namespace bbob {
    class AttractiveSectorFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit AttractiveSectorFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditioning_;

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
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<AttractiveSectorFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };
  
    //
    // Implementation
    //

    inline AttractiveSectorFunction::AttractiveSectorFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(std::sqrt(10.0))) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void AttractiveSectorFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    inline double AttractiveSectorFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = rotationQ_ * (parameterConditioning_ % parameter);
      z.elem(arma::find(z % parameterTranslation_ > 0.0)) *= 100.0;

      return std::pow(getOscillatedValue(std::pow(arma::norm(z), 2.0)), 0.9);
    }

    inline std::string AttractiveSectorFunction::toString() const noexcept {
      return "bbob_attractive_sector_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::AttractiveSectorFunction);
#endif
