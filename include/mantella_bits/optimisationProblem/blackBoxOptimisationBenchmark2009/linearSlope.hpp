namespace mant {
  namespace bbob2009 {
    class LinearSlope : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit LinearSlope(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setReflection(
            const bool reflection) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> xOpt_;
        arma::Col<double> scaling_;
        double partialObjectiveValue_;

        arma::Col<double> reflection_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("reflection", reflection_));
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
          archive(cereal::make_nvp("reflection", construct->reflection_));
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
      setReflection(std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? true : false);
    }

    inline void LinearSlope::setReflection(
        const bool reflection) noexcept {
      if (reflection) {
        reflection_ = -arma::ones<arma::Col<double>>(numberOfDimensions_);
      } else {
        reflection_ = arma::ones<arma::Col<double>>(numberOfDimensions_);
      }

      xOpt_ = 5.0 * reflection_;
      scaling_ = arma::sign(reflection_) % getScaledTransformation(10.0);
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
