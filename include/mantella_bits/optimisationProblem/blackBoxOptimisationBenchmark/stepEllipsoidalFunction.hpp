namespace mant {
  namespace bbob {
    template <typename T = double>
    class StepEllipsoidalFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit StepEllipsoidalFunction(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> firstParameterConditioning_;
        const arma::Col<T> secondParameterConditioning_;

        arma::Mat<T> rotationQ_;

        T getSoftConstraintsValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

        T getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

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
            cereal::construct<StepEllipsoidalFunction>& construct) noexcept {
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

    template <typename T>
    StepEllipsoidalFunction<T>::StepEllipsoidalFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        firstParameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))),
        secondParameterConditioning_(this->getParameterConditioning(static_cast<T>(100.0L))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    void StepEllipsoidalFunction<T>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T>
    T StepEllipsoidalFunction<T>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return this->getBoundConstraintsValue(parameter);
    }
    
    template <typename T>
    T StepEllipsoidalFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& s = firstParameterConditioning_ % parameter;

      arma::Col<T> z = s;
      for (std::size_t n = 0; n < z.n_elem; ++n) {
        const T& value = s(n);

        if (std::abs(value) > static_cast<T>(0.5L)) {
          z(n) = std::round(value);
        } else {
          z(n) = std::round(value * static_cast<T>(10.0L)) / static_cast<T>(10.0L);
        }
      }

      return static_cast<T>(0.1L) * std::max(std::abs(s(0)) / static_cast<T>(10000.0L), arma::dot(secondParameterConditioning_, arma::square(rotationQ_ * z)));
    }

    template <typename T>
    std::string StepEllipsoidalFunction<T>::toString() const noexcept {
      return "bbob_step_ellipsoidal_function";
    }
  }
}
