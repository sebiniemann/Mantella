namespace mant {
  namespace bbob {
    template <typename T = double>
    class LinearSlope : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit LinearSlope(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;
        const T f0_;

        T getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LinearSlope>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
        }
#endif
    };

    //
    // Implementation
    //

    template <typename T>
    LinearSlope<T>::LinearSlope(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(static_cast<T>(10.0L))),
        f0_(static_cast<T>(5.0L) * arma::accu(parameterConditioning_)) {
      this->setParameterRotation(arma::eye<arma::Mat<T>>(this->numberOfDimensions_, this->numberOfDimensions_) * (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? static_cast<T>(1.0L) : static_cast<T>(-1.0L)));
    }

    template <typename T>
    T LinearSlope<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> z = parameter;
      z.elem(arma::find(parameter >= static_cast<T>(5.0L))).fill(static_cast<T>(5.0L));

      return f0_ - arma::dot(parameterConditioning_, z);
    }

    template <typename T>
    std::string LinearSlope<T>::toString() const noexcept {
      return "bbob_linear_slope";
    }
  }
}
