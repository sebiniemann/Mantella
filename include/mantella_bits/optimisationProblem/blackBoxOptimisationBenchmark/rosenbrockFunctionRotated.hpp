namespace mant {
  namespace bbob {
    template <typename T = double>
    class RosenbrockFunctionRotated : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit RosenbrockFunctionRotated(
            const std::size_t numberOfDimensions) noexcept;

        std::string toString() const noexcept override;

      protected:
        const T max_;

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
            cereal::construct<RosenbrockFunctionRotated>& construct) noexcept {
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
    RosenbrockFunctionRotated<T>::RosenbrockFunctionRotated(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        max_(std::max(static_cast<T>(1.0L), std::sqrt(static_cast<T>(this->numberOfDimensions_)) / static_cast<T>(8.0L))) {
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    T RosenbrockFunctionRotated<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = max_ * parameter + static_cast<T>(0.5L);

      return static_cast<T>(100.0L) * std::pow(arma::norm(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1)), static_cast<T>(2.0L)) + std::pow(arma::norm(z.head(z.n_elem - 1) - static_cast<T>(1.0L)), static_cast<T>(2.0L));
    }

    template <typename T>
    std::string RosenbrockFunctionRotated<T>::toString() const noexcept {
      return "bbob_rosenbrock_function_rotated";
    }
  }
}
