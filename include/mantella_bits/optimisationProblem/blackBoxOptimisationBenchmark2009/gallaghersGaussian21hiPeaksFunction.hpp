namespace mant {
  namespace bbob2009 {
    class GallaghersGaussian21hiPeaksFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit GallaghersGaussian21hiPeaksFunction(
            const unsigned int& numberOfDimensions) noexcept;

        GallaghersGaussian21hiPeaksFunction(const GallaghersGaussian21hiPeaksFunction&) = delete;
        GallaghersGaussian21hiPeaksFunction& operator=(const GallaghersGaussian21hiPeaksFunction&) = delete;

        inline std::string to_string() const noexcept override;

      protected:
        arma::Col<double> weight_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
        friend class cereal::access;

        // TODO Add a default constructor to calculate the weight and remove it from cereal

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("deltaC21", deltaC21_));
          archive(cereal::make_nvp("localOptimaY21", localOptimaY21_));
          archive(cereal::make_nvp("weight", weight_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<GallaghersGaussian21hiPeaksFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("deltaC21", construct->deltaC21_));
          archive(cereal::make_nvp("localOptimaY21", construct->localOptimaY21_));
          archive(cereal::make_nvp("weight", construct->weight_));
        }
#endif
    };

    inline GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        weight_(21) {
      weight_.at(0) = 10.0;
      for (std::size_t n = 1; n < weight_.n_elem; ++n) {
        weight_.at(n) = 1.1 + 8.0 * static_cast<double>(n - 1) / 19.0;
      }
    }

    inline double GallaghersGaussian21hiPeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double maximalValue = std::numeric_limits<double>::lowest();
      for (unsigned int k = 0; k < 21; ++k) {
        const arma::Col<double>& parameterTransposition = parameter - localOptimaY21_.col(k);
        maximalValue = std::max(maximalValue, weight_.at(k) * std::exp(-1.0 / (2.0 * static_cast<double>(numberOfDimensions_)) * arma::dot(parameterTransposition, rotationR_.t() * arma::diagmat(deltaC21_.col(k)) * rotationR_ * parameterTransposition)));
      }

      return std::pow(getOscillationTransformation(10.0 - maximalValue), 2.0) + getPenality(parameter);
    }

    inline std::string GallaghersGaussian21hiPeaksFunction::to_string() const noexcept {
      return "GallaghersGaussian21hiPeaksFunction";
    }
  }
}

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
// CEREAL_REGISTER_TYPE(mant::bbob2009::GallaghersGaussian21hiPeaksFunction);
#endif
