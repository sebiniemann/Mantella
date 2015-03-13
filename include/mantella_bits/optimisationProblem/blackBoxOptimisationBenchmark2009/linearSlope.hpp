namespace mant {
  namespace bbob2009 {
    class LinearSlope : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit LinearSlope(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setParameterReflection(
            const bool Parameterreflection) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> parameterReflection_;

        arma::Col<double> parameterConditioning_;
        double f0_;

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
          archive(cereal::make_nvp("parameterConditioning", parameterConditioning_));
          archive(cereal::make_nvp("f0", f0_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LinearSlope>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("parameterReflection", construct->parameterReflection_));
          archive(cereal::make_nvp("parameterConditioning", construct->parameterConditioning_));
          archive(cereal::make_nvp("f0", construct->f0_));
        }
#endif
    };

    //
    // Implementation
    //

    inline LinearSlope::LinearSlope(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      setParameterReflection(std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? true : false);
    }

    inline void LinearSlope::setParameterReflection(
        const bool parameterReflection) noexcept {
      parameterReflection_ = arma::zeros<arma::Col<double>>(numberOfDimensions_) + (parameterReflection ? 5.0 : -5.0);

      parameterConditioning_ = arma::sign(parameterReflection_) % getParameterConditioning(10.0);
      f0_ = 5.0 * arma::accu(arma::abs(parameterConditioning_));
    }

    inline double LinearSlope::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = parameter;

      const arma::Col<unsigned int>& outOfBound = arma::find(parameterReflection_ % z >= 25.0);
      z.elem(outOfBound) = parameterReflection_.elem(outOfBound);

      return f0_ - arma::dot(parameterConditioning_, z);
    }

    inline std::string LinearSlope::toString() const noexcept {
      return "linear-slope";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob2009::LinearSlope);
#endif
