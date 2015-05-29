namespace mant {
  namespace bbob {
    template <typename T = double>
    class DifferentPowersFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit DifferentPowersFunction(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
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
            cereal::construct<DifferentPowersFunction>& construct) noexcept {
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
    DifferentPowersFunction<T>::DifferentPowersFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    T DifferentPowersFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = arma::abs(parameter);
      return arma::norm(z % this->getConditionedParameter(arma::square(z)));
    }

    template <typename T>
    std::string DifferentPowersFunction<T>::toString() const noexcept {
      return "bbob_different_powers_function";
    }
  }
}