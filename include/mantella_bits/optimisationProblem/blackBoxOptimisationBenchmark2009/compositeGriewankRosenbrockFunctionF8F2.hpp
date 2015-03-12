namespace mant {
  namespace bbob2009 {
    class CompositeGriewankRosenbrockFunctionF8F2 : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit CompositeGriewankRosenbrockFunctionF8F2(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setRotationR(
            const arma::Mat<double>& rotationR);

        inline std::string toString() const noexcept override;

      protected:
        const double max_;

        arma::Mat<double> rotationR_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<CompositeGriewankRosenbrockFunctionF8F2>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
        }
#endif
    };

    //
    // Implementation
    //

    inline CompositeGriewankRosenbrockFunctionF8F2::CompositeGriewankRosenbrockFunctionF8F2(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        max_(std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0)) {
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void CompositeGriewankRosenbrockFunctionF8F2::setRotationR(
        const arma::Mat<double>& rotationR) {
      checkDimensionCompatible("The number of rows", rotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", rotationR);

      rotationR_ = rotationR;
    }

    inline double CompositeGriewankRosenbrockFunctionF8F2::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = max_ * rotationR_ * parameter + 0.5;
      const arma::Col<double>& s = 100.0 * arma::square(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1)) + arma::square(1.0 - z.head(z.n_elem - 1));

      return 10.0 * (arma::mean(s / 4000.0 - arma::cos(s)) + 1);
    }

    inline std::string CompositeGriewankRosenbrockFunctionF8F2::toString() const noexcept {
      return "composite-griewank-rosenbrock-function-f8f2";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::CompositeGriewankRosenbrockFunctionF8F2);
#endif
