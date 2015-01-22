namespace mant {
  namespace bbob2009 {
    class SharpRidgeFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        SharpRidgeFunction(const SharpRidgeFunction&) = delete;
        SharpRidgeFunction& operator=(const SharpRidgeFunction&) = delete;

        inline std::string to_string() const noexcept override;

      protected:
        const arma::Col<double> delta_ = getScaling(std::sqrt(10.0));

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));;
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<SharpRidgeFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline double SharpRidgeFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = rotationQ_ * (delta_ % (rotationR_ * (parameter - translation_)));
      return std::pow(z.at(0), 2.0) + 100.0 * arma::norm(z.tail(z.n_elem - 1));
    }

    inline std::string SharpRidgeFunction::to_string() const noexcept {
      return "SharpRidgeFunction";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::SharpRidgeFunction);
#endif
