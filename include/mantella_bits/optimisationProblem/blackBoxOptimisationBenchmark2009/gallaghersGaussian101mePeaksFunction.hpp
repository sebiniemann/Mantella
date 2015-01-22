namespace mant {
  namespace bbob2009 {
    class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        explicit GallaghersGaussian101mePeaksFunction(
            const unsigned int& numberOfDimensions) ;

        GallaghersGaussian101mePeaksFunction(const GallaghersGaussian101mePeaksFunction&) = delete;
        GallaghersGaussian101mePeaksFunction& operator=(const GallaghersGaussian101mePeaksFunction&) = delete;

        std::string to_string() const  override;

      protected:
        arma::Col<double> weight_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const  override;

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
        friend class cereal::access;

        // TODO Add a default constructor to calculate the weight and remove it from cereal

        template <typename Archive>
        void serialize(
            Archive& archive)  {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("deltaC101", deltaC101_));
          archive(cereal::make_nvp("localOptimaY101", localOptimaY101_));
          archive(cereal::make_nvp("weight", weight_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<GallaghersGaussian101mePeaksFunction>& construct)  {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("deltaC101", construct->deltaC101_));
          archive(cereal::make_nvp("localOptimaY101", construct->localOptimaY101_));
          archive(cereal::make_nvp("weight", construct->weight_));
        }
#endif
    };
  }
}

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
// CEREAL_REGISTER_TYPE(mant::bbob2009::GallaghersGaussian101mePeaksFunction);
#endif
