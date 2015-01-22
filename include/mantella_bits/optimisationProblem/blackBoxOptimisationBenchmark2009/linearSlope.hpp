namespace mant {
  namespace bbob2009 {
    class LinearSlope : public BlackBoxOptimisationBenchmark2009 {
      public:
        explicit LinearSlope(
            const unsigned int& numberOfDimensions) ;

        LinearSlope(const LinearSlope&) = delete;
        LinearSlope& operator=(const LinearSlope&) = delete;

        void setOne(
            const arma::Col<double>& one) override;

        std::string to_string() const  override;

      protected:
        arma::Col<double> xOpt_;
        arma::Col<double> scaling_;
        double partiallyObjectiveValue_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const  override;

        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive)  {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("one", one_));
          archive(cereal::make_nvp("xOpt", xOpt_));
          archive(cereal::make_nvp("scaling", scaling_));
          archive(cereal::make_nvp("partiallyObjectiveValue", partiallyObjectiveValue_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LinearSlope>& construct)  {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("one", construct->one_));
          archive(cereal::make_nvp("xOpt", construct->xOpt_));
          archive(cereal::make_nvp("scaling", construct->scaling_));
          archive(cereal::make_nvp("partiallyObjectiveValue", construct->partiallyObjectiveValue_));
        }
    };
  }
}
