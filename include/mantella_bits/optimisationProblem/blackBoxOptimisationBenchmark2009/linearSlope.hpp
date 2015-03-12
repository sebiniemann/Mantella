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
        arma::Col<double> xOpt_;
        arma::Col<double> scaling_;
        double partialObjectiveValue_;

        arma::Col<double> Parameterreflection_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("Parameterreflection", Parameterreflection_));
          archive(cereal::make_nvp("xOpt", xOpt_));
          archive(cereal::make_nvp("scaling", scaling_));
          archive(cereal::make_nvp("partialObjectiveValue", partialObjectiveValue));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LinearSlope>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("Parameterreflection", construct->Parameterreflection_));
          archive(cereal::make_nvp("xOpt", construct->xOpt_));
          archive(cereal::make_nvp("scaling", construct->scaling_));
          archive(cereal::make_nvp("partialObjectiveValue", construct->partialObjectiveValue));
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
        const bool Parameterreflection) noexcept {
      if (Parameterreflection) {
        Parameterreflection_ = -arma::ones<arma::Col<double>>(numberOfDimensions_);
      } else {
        Parameterreflection_ = arma::ones<arma::Col<double>>(numberOfDimensions_);
      }

      xOpt_ = 5.0 * Parameterreflection_;
      scaling_ = arma::sign(Parameterreflection_) % getScaledTransformation(10.0);
      partialObjectiveValue_ = 5.0 * arma::accu(arma::abs(scaling_));
    }

    inline double LinearSlope::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = parameter;

      const arma::Col<unsigned int>& outOfBound = arma::find(xOpt_ % z >= 25.0);
      z.elem(outOfBound) = xOpt_.elem(outOfBound);

      return partialObjectiveValue_ - arma::dot(scaling_, z);
    }

    inline std::string LinearSlope::toString() const noexcept {
      return "linear-slope";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::LinearSlope);
#endif
