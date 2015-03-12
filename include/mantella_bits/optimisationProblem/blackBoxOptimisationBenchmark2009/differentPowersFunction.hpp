namespace mant {
  namespace bbob2009 {
    class DifferentPowersFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit DifferentPowersFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setLocalParameterTranslation(
            const arma::Col<double>& localParameterTranslation);

        inline void setRotationR(
            const arma::Mat<double>& rotationR);

        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> localParameterTranslation_;
        arma::Mat<double> rotationR_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("localParameterTranslation", localParameterTranslation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<DifferentPowersFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("localParameterTranslation", construct->localParameterTranslation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
        }
#endif
    };

    //
    // Implementation
    //

    inline DifferentPowersFunction::DifferentPowersFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void DifferentPowersFunction::setLocalParameterTranslation(
        const arma::Col<double>& localParameterTranslation) {
      checkDimensionCompatible("The number of elements", localParameterTranslation.n_elem, "the number of dimensions", numberOfDimensions_);

      localParameterTranslation_ = localParameterTranslation;
    }

    inline void DifferentPowersFunction::setRotationR(
        const arma::Mat<double>& rotationR) {
      checkDimensionCompatible("The number of rows", rotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", rotationR);

      rotationR_ = rotationR;
    }

    inline double DifferentPowersFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = arma::abs(rotationR_ * (parameter - localParameterTranslation_));
      return arma::norm(z % getScaledTransformation(arma::square(z)));
    }

    inline std::string DifferentPowersFunction::toString() const noexcept {
      return "different-powers-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::DifferentPowersFunction);
#endif
