namespace mant {
  namespace bbob {
    template <typename T = double>
    class SchaffersF7Function : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit SchaffersF7Function(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

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
            cereal::construct<SchaffersF7Function>& construct) noexcept {
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
    SchaffersF7Function<T>::SchaffersF7Function(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    void SchaffersF7Function<T>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T>
    T SchaffersF7Function<T>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return static_cast<T>(10.0L) * this->getBoundConstraintsValue(parameter);
    }

    template <typename T>
    T SchaffersF7Function<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& s = arma::square(parameterConditioning_ % (rotationQ_ * this->getAsymmetricParameter(static_cast<T>(0.5L), parameter)));
      const arma::Col<T>& z = arma::pow(s.head(s.n_elem - 1) + s.tail(s.n_elem - 1), static_cast<T>(0.25L));

      return std::pow(arma::mean(z % (static_cast<T>(1.0L) + arma::square(arma::sin(static_cast<T>(50.0L) * arma::pow(z, static_cast<T>(0.4L)))))), static_cast<T>(2.0L));
    }

    template <typename T>
    std::string SchaffersF7Function<T>::toString() const noexcept {
      return "bbob_schaffers_f7_function";
    }
  }
}
