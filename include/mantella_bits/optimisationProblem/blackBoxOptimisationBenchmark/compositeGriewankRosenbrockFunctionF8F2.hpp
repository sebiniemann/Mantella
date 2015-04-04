namespace mant {
  namespace bbob {
    class CompositeGriewankRosenbrockFunctionF8F2 : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit CompositeGriewankRosenbrockFunctionF8F2(
            const unsigned int numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

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
            cereal::construct<CompositeGriewankRosenbrockFunctionF8F2>& construct) noexcept {
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

    inline CompositeGriewankRosenbrockFunctionF8F2::CompositeGriewankRosenbrockFunctionF8F2(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      // A vector with all elements set to max(1, numberOfDimensions / 8).
      setParameterScaling(arma::zeros<arma::Col<double>>(numberOfDimensions_) + std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0));
      // A vector with all elements set to 0.5.
      setParameterTranslation(arma::zeros<arma::Col<double>>(numberOfDimensions_) + 0.5);
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline double CompositeGriewankRosenbrockFunctionF8F2::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = 100.0 * arma::square(arma::square(parameter.head(parameter.n_elem - 1)) - parameter.tail(parameter.n_elem - 1)) + arma::square(1.0 - parameter.head(parameter.n_elem - 1));

      return 10.0 * (arma::mean(z / 4000.0 - arma::cos(z)) + 1);
    }

    inline std::string CompositeGriewankRosenbrockFunctionF8F2::toString() const noexcept {
      return "bbob_composite_griewank_rosenbrock_function_f8f2";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::CompositeGriewankRosenbrockFunctionF8F2);
#endif
