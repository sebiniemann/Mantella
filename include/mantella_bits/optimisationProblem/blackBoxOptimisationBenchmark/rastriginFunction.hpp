namespace mant {
  namespace bbob {
    template <typename T = double>
    class RastriginFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit RastriginFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

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
            cereal::construct<RastriginFunction>& construct) noexcept {
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
    RastriginFunction<T>::RastriginFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
    }

    template <typename T>
    T RastriginFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = parameterConditioning_ % this->getAsymmetricParameter(static_cast<T>(0.2L), this->getOscillatedParameter(parameter));

      return static_cast<T>(10.0L) * (static_cast<T>(this->numberOfDimensions_) - arma::accu(arma::cos(static_cast<T>(2.0L) * arma::datum::pi * z))) + std::pow(arma::norm(z), static_cast<T>(2.0L));
    }

    template <typename T>
    std::string RastriginFunction<T>::toString() const noexcept {
      return "bbob_rastrigin_function";
    }
  }
}
