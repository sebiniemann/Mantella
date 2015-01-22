namespace mant {
  namespace bbob2009 {
    class DiscusFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        DiscusFunction(const DiscusFunction&) = delete;
        DiscusFunction& operator=(const DiscusFunction&) = delete;

        inline std::string to_string() const noexcept override;

      protected:
        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<DiscusFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
        }
#endif
    };

    //
    // Implementation
    //

    inline double DiscusFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = arma::square(getOscillationTransformation(rotationR_ * (parameter - translation_)));
      return 1000000.0 * z.at(0) + arma::accu(z.tail(z.n_elem - 1));
    }

    inline std::string DiscusFunction::to_string() const noexcept {
      return "DiscusFunction";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::DiscusFunction);
#endif
