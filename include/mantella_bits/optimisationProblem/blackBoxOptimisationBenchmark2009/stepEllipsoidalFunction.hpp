namespace mant {
  namespace bbob2009 {
    class StepEllipsoidalFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit StepEllipsoidalFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setLocalParameterTranslation(
            const arma::Col<double>& localParameterTranslation);

        inline void setRotationR(
            const arma::Mat<double>& rotationR);

        inline void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> localParameterTranslation_;
        arma::Mat<double> rotationR_;
        arma::Mat<double> rotationQ_;
        
        const arma::Col<double> firstScaling_;
        const arma::Col<double> secondScaling_;

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
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<StepEllipsoidalFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("localParameterTranslation", construct->localParameterTranslation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline StepEllipsoidalFunction::StepEllipsoidalFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        firstScaling_(getScaledTransformation(std::sqrt(10.0))),
        secondScaling_(getScaledTransformation(100)) {
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void StepEllipsoidalFunction::setLocalParameterTranslation(
        const arma::Col<double>& localParameterTranslation) {
      checkDimensionCompatible("The number of elements", localParameterTranslation.n_elem, "the number of dimensions", numberOfDimensions_);

      localParameterTranslation_ = localParameterTranslation;
    }

    inline void StepEllipsoidalFunction::setRotationR(
        const arma::Mat<double>& rotationR) {
      checkDimensionCompatible("The number of rows", rotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", rotationR);

      rotationR_ = rotationR;
    }

    inline void StepEllipsoidalFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      checkDimensionCompatible("The number of rows", rotationQ.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", rotationQ);

      rotationQ_ = rotationQ;
    }
    
    inline double StepEllipsoidalFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& zHat = firstScaling_ % (rotationR_ * (parameter - localParameterTranslation_));

      arma::Col<double> zTilde(zHat);
      for (std::size_t n = 0; n < zTilde.n_elem; ++n) {
        const double& value = zHat(n);

        if (std::abs(value) > 0.5) {
          zTilde(n) = std::round(value);
        } else {
          zTilde(n) = std::round(value * 10.0) / 10.0;
        }
      }

      return 0.1 * std::max(std::abs(zHat(0)) / 10000.0, arma::dot(secondScaling_, arma::square(rotationQ_ * zTilde))) + getPenality(parameter);
    }

    inline std::string StepEllipsoidalFunction::toString() const noexcept {
      return "step-ellipsoidal-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::StepEllipsoidalFunction);
#endif
