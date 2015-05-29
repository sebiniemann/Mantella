namespace mant {
  namespace bbob {
    template <typename T = double>
    class EllipsoidalFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit EllipsoidalFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

        T getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<EllipsoidalFunction>& construct) noexcept {
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
    EllipsoidalFunction<T>::EllipsoidalFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(static_cast<T>(1000000.0L))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
    }

    template <typename T>
    T EllipsoidalFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return arma::dot(parameterConditioning_, arma::square(this->getOscillatedParameter(parameter)));
    }

    template <typename T>
    std::string EllipsoidalFunction<T>::toString() const noexcept {
      return "bbob_ellipsoidal_function";
    }
  }
}
