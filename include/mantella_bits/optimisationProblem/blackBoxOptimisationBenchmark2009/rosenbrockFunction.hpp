namespace mant {
  namespace bbob2009 {
    class RosenbrockFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit RosenbrockFunction(
            const unsigned int& numberOfDimensions) noexcept;

        RosenbrockFunction(const RosenbrockFunction&) = delete;
        RosenbrockFunction& operator=(const RosenbrockFunction&) = delete;

        inline void setTranslation(
            const arma::Col<double>& translation) override;

        inline std::string to_string() const noexcept override;

      protected:
        const double max_ = std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0);

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<RosenbrockFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
        }
#endif
    };

    //
    // Implementation
    //

    inline RosenbrockFunction::RosenbrockFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      setTranslation(translation_);
    }

    inline void RosenbrockFunction::setTranslation(
        const arma::Col<double>& translation) {
      if (translation.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the translation (" + std::to_string(translation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      translation_ = 0.75 * translation;
    }

    inline double RosenbrockFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = max_ * (parameter - translation_) + 1.0;

      return 100.0 * arma::accu(arma::square(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1))) + arma::accu(arma::square(z.head(z.n_elem - 1) - 1.0));
    }

    inline std::string RosenbrockFunction::to_string() const noexcept {
      return "RosenbrockFunction";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::RosenbrockFunction);
#endif
