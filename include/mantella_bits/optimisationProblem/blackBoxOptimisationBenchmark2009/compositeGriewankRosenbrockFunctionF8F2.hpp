namespace mant {
  namespace bbob2009 {
    class CompositeGriewankRosenbrockFunctionF8F2 : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        CompositeGriewankRosenbrockFunctionF8F2(const CompositeGriewankRosenbrockFunctionF8F2&) = delete;
        CompositeGriewankRosenbrockFunctionF8F2& operator=(const CompositeGriewankRosenbrockFunctionF8F2&) = delete;

        std::string to_string() const  override;

      protected:
        const double max_ = std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0);

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const  override;

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive)  {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<CompositeGriewankRosenbrockFunctionF8F2>& construct)  {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
        }
#endif
    };
  }
}

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
// CEREAL_REGISTER_TYPE(mant::bbob2009::CompositeGriewankRosenbrockFunctionF8F2);
#endif
