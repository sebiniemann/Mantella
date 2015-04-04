namespace mant {
  namespace bbob {
    class LinearSlope : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit LinearSlope(
            const unsigned int numberOfDimensions) noexcept;

        inline void setObjectiveFunctionRotation(
            const arma::Col<double>& objectiveFunctionRotation) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditioning_;
        const double f0_;

        arma::Col<double> objectiveFunctionRotation_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("objectiveFunctionRotation", objectiveFunctionRotation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LinearSlope>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
          archive(cereal::make_nvp("objectiveFunctionRotation", construct->objectiveFunctionRotation_));
        }
#endif
    };

    //
    // Implementation
    //

    inline LinearSlope::LinearSlope(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(10.0)),
        f0_(5.0 * arma::accu(parameterConditioning_)) {
      setObjectiveFunctionRotation(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 1.0 : -1.0));
    }

    inline void LinearSlope::setObjectiveFunctionRotation(
        const arma::Col<double>& objectiveFunctionRotation) noexcept {
      objectiveFunctionRotation_ = objectiveFunctionRotation;
    }

    inline double LinearSlope::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = parameter;

      const arma::Col<unsigned int>& outOfBound = arma::find(objectiveFunctionRotation_ % z >= 5.0);
      z.elem(outOfBound) = objectiveFunctionRotation_.elem(outOfBound) * 5;

      return f0_ - arma::dot(arma::sign(objectiveFunctionRotation_) % parameterConditioning_, z);
    }

    inline std::string LinearSlope::toString() const noexcept {
      return "bbob_linear_slope";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::LinearSlope);
#endif
