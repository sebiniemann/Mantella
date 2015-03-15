namespace mant {
  namespace bbob2009 {
    class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit GallaghersGaussian101mePeaksFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline virtual void setParameterRotationR(
            const arma::Mat<double>& parameterRotationR);

        inline virtual void setLocalParameterConditioning(
            const arma::Col<double>& localParameterConditioning);

        inline virtual void setLocalParameterTranslation(
            const arma::Mat<double>& localParameterTranslation);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double>::fixed<101> weight_;

        arma::Mat<double> parameterRotationR_;
        arma::Mat<double> localParameterConditioning_;
        arma::Mat<double> localParameterTranslation_;

        inline arma::Col<double> getRandomLocalParameterConditioning() const noexcept;

        inline arma::Mat<double> getRandomLocalParameterTranslation() const noexcept;

        inline double getSoftConstraintsValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("parameterRotationR", parameterRotationR_));
          archive(cereal::make_nvp("localParameterConditioning", localParameterConditioning_));
          archive(cereal::make_nvp("localParameterTranslation", localParameterTranslation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<GallaghersGaussian101mePeaksFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("parameterRotationR", construct->parameterRotationR_));
          archive(cereal::make_nvp("localParameterConditioning", construct->localParameterConditioning_));
          archive(cereal::make_nvp("localParameterTranslation", construct->localParameterTranslation_));
        }
#endif
    };

    //
    // Implementation
    //

    inline GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        weight_({10, 1.1, 1.180808080808081, 1.261616161616162, 1.342424242424243, 1.423232323232323, 1.504040404040404, 1.584848484848485, 1.665656565656566, 1.746464646464647, 1.827272727272727, 1.908080808080808, 1.988888888888889, 2.06969696969697, 2.150505050505051, 2.231313131313131, 2.312121212121212, 2.392929292929293, 2.473737373737374, 2.554545454545455, 2.635353535353535, 2.716161616161616, 2.796969696969697, 2.877777777777778, 2.958585858585859, 3.03939393939394, 3.12020202020202, 3.201010101010101, 3.281818181818182, 3.362626262626263, 3.443434343434344, 3.524242424242424, 3.605050505050505, 3.685858585858586, 3.766666666666667, 3.847474747474747, 3.928282828282828, 4.00909090909091, 4.08989898989899, 4.170707070707071, 4.251515151515152, 4.332323232323232, 4.413131313131313, 4.493939393939394, 4.574747474747475, 4.655555555555555, 4.736363636363636, 4.817171717171718, 4.897979797979798, 4.978787878787879, 5.05959595959596, 5.140404040404041, 5.221212121212121, 5.302020202020202, 5.382828282828283, 5.463636363636363, 5.544444444444444, 5.625252525252526, 5.706060606060607, 5.786868686868686, 5.867676767676768, 5.948484848484849, 6.029292929292929, 6.11010101010101, 6.190909090909091, 6.271717171717173, 6.352525252525252, 6.433333333333334, 6.514141414141415, 6.594949494949494, 6.675757575757576, 6.756565656565657, 6.837373737373737, 6.918181818181818, 6.998989898989899, 7.079797979797981, 7.16060606060606, 7.241414141414142, 7.322222222222223, 7.403030303030302, 7.483838383838384, 7.564646464646465, 7.645454545454546, 7.726262626262626, 7.807070707070707, 7.887878787878789, 7.968686868686868, 8.049494949494949, 8.130303030303031, 8.21111111111111, 8.291919191919192, 8.372727272727273, 8.453535353535354, 8.534343434343434, 8.615151515151515, 8.695959595959597, 8.776767676767676, 8.857575757575757, 8.938383838383839, 9.01919191919192, 9.1}) {
      setParameterRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setLocalParameterConditioning(getRandomLocalParameterConditioning());
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
    }

    inline void GallaghersGaussian101mePeaksFunction::setParameterRotationR(
        const arma::Mat<double>& parameterRotationR) {
      checkDimensionCompatible("The number of rows", parameterRotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", parameterRotationR);

      parameterRotationR_ = parameterRotationR;
    }

    inline void GallaghersGaussian101mePeaksFunction::setLocalParameterConditioning(
        const arma::Col<double>& localParameterConditioning) {
      checkDimensionCompatible("The number of elements", localParameterConditioning.n_elem, "the number of peaks", 101);

      localParameterConditioning_.set_size(numberOfDimensions_, 101);
      for (std::size_t n = 0; n < localParameterConditioning.n_elem; ++n) {
        const double& localParameterConditioningValue = std::pow(10.0, localParameterConditioning(n) / 33.0);
        localParameterConditioning_.col(n) = getParameterConditioning(localParameterConditioningValue) / std::sqrt(localParameterConditioningValue);
      }
    }

    inline void GallaghersGaussian101mePeaksFunction::setLocalParameterTranslation(
        const arma::Mat<double>& localParameterTranslation) {
      checkDimensionCompatible("The number of rows", localParameterTranslation.n_rows, "the number of dimensions", numberOfDimensions_);
      checkDimensionCompatible("The number of columns", localParameterTranslation.n_cols, "the number of peaks", 101);

      localParameterTranslation_ = localParameterTranslation;
    }

    inline arma::Col<double> GallaghersGaussian101mePeaksFunction::getRandomLocalParameterConditioning() const noexcept {
      arma::Col<double> localParameterConditioning(101);
      localParameterConditioning(0) = 49.5;
      localParameterConditioning.tail(100) = arma::conv_to<arma::Col<double>>::from(getRandomPermutation(100));

      return localParameterConditioning;
    }

    inline arma::Mat<double> GallaghersGaussian101mePeaksFunction::getRandomLocalParameterTranslation() const noexcept {
      arma::Mat<double> localParameterTranslation = arma::randu<arma::Mat<double>>(numberOfDimensions_, 101) * 10.0 - 5.0;
      localParameterTranslation.col(0) = 0.8 * localParameterTranslation.col(0);

      return localParameterTranslation;
    }

    inline double GallaghersGaussian101mePeaksFunction::getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return getBoundConstraintsValue(parameter);
    }

    inline double GallaghersGaussian101mePeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double maximalValue = std::numeric_limits<double>::lowest();
      for (unsigned int k = 0; k < 101; ++k) {
        const arma::Col<double>& locallyTranslatedParameter = parameter - localParameterTranslation_.col(k);
        maximalValue = std::max(maximalValue, weight_(k) * std::exp(-1.0 / (2.0 * static_cast<double>(numberOfDimensions_)) * arma::dot(locallyTranslatedParameter, parameterRotationR_.t() * arma::diagmat(localParameterConditioning_.col(k)) * parameterRotationR_ * locallyTranslatedParameter)));
      }

      return std::pow(getOscillatedValue(10.0 - maximalValue), 2.0);
    }

    inline std::string GallaghersGaussian101mePeaksFunction::toString() const noexcept {
      return "gallaghers-gaussian-101me-peaks-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob2009::GallaghersGaussian101mePeaksFunction);
#endif
