namespace mant {
  namespace bbob2009 {
    class SchwefelFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        SchwefelFunction(const SchwefelFunction&) = delete;
        SchwefelFunction& operator=(const SchwefelFunction&) = delete;

        std::string to_string() const  override;

      protected:
        arma::Col<double> delta_ = getScaling(std::sqrt(10));

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const  override;

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive)  {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("one", one_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<SchwefelFunction>& construct)  {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("one", construct->one_));
        }
#endif
    };
  }
}

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
// CEREAL_REGISTER_TYPE(mant::bbob2009::SchwefelFunction);
#endif
