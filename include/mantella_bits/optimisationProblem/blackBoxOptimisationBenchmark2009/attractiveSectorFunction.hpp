namespace mant {
  namespace bbob2009 {
    class AttractiveSectorFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        AttractiveSectorFunction(const AttractiveSectorFunction&) = delete;
        AttractiveSectorFunction& operator=(const AttractiveSectorFunction&) = delete;

        std::string to_string() const  override;

      protected:
        const arma::Col<double> delta_ = getScaling(std::sqrt(10.0));

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const  override;

        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive)  {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<AttractiveSectorFunction>& construct)  {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
    };
  }
}
