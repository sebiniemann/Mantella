namespace mant {
  namespace bbob2009 {
    class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit GallaghersGaussian101mePeaksFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> weight_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
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
            cereal::construct<GallaghersGaussian101mePeaksFunction>& construct) noexcept {
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

    //
    // Implementation
    //

    inline GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        weight_(101) {
      weight_.at(0) = 10.0;
      for (std::size_t n = 1; n < weight_.n_elem; ++n) {
        weight_.at(n) = 1.1 + 8.0 * static_cast<double>(n - 1) / 99.0;
      }
    }

    inline double GallaghersGaussian101mePeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double maximalValue = std::numeric_limits<double>::lowest();
      for (unsigned int k = 0; k < 101; ++k) {
        const arma::Col<double>& parameterTransposition = parameter - localOptimaY101_.col(k);
        maximalValue = std::max(maximalValue, weight_.at(k) * std::exp(-1.0 / (2.0 * static_cast<double>(numberOfDimensions_)) * dot(parameterTransposition, rotationR_.t() * arma::diagmat(deltaC101_.col(k)) * rotationR_ * parameterTransposition)));
      }

      return std::pow(getOscillationTransformation(10.0 - maximalValue), 2.0) + getPenality(parameter);
    }

    inline std::string GallaghersGaussian101mePeaksFunction::toString() const noexcept {
      return "GallaghersGaussian101mePeaksFunction";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::GallaghersGaussian101mePeaksFunction);
#endif
