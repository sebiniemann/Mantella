namespace mant {
  namespace bbob2009 {
    class RosenbrockFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit RosenbrockFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        const double max_;

        arma::Col<double> localParameterTranslation_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<RosenbrockFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
        }
#endif
    };

    //
    // Implementation
    //

    inline RosenbrockFunction::RosenbrockFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        max_(std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0)) {
      setParameterTranslation(0.75 * getRandomParameterTranslation());
    }

    inline double RosenbrockFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = max_ * parameter + 1.0;

      return 100.0 * std::pow(arma::norm(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1)), 2) + std::pow(arma::norm(z.head(z.n_elem - 1) - 1.0), 2);
    }

    inline std::string RosenbrockFunction::toString() const noexcept {
      return "rosenbrock-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob2009::RosenbrockFunction);
#endif
