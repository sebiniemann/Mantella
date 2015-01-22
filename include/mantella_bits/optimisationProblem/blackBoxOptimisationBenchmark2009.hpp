namespace mant {
  namespace bbob2009 {
    class BlackBoxOptimisationBenchmark2009 : public OptimisationProblem<double> {
      public:
        explicit BlackBoxOptimisationBenchmark2009(
            const unsigned int& numberOfDimensions) ;

        BlackBoxOptimisationBenchmark2009(const BlackBoxOptimisationBenchmark2009&) = delete;
        BlackBoxOptimisationBenchmark2009& operator=(const BlackBoxOptimisationBenchmark2009&) = delete;

        virtual void setTranslation(
            const arma::Col<double>& translation);
        virtual void setOne(
            const arma::Col<double>& one);
        virtual void setRotationR(
            const arma::Mat<double>& rotationR);
        virtual void setRotationQ(
            const arma::Mat<double>& rotationQ);
        virtual void setDeltaC101(
            const arma::Mat<double>& deltaC101);
        virtual void setLocalOptimaY101(
            const arma::Mat<double>& localOptimaY101);
        virtual void setDeltaC21(
            const arma::Mat<double>& deltaC21);
        virtual void setLocalOptimaY21(
            const arma::Mat<double>& localOptimaY21);

        virtual ~BlackBoxOptimisationBenchmark2009() = default;

      protected:
        arma::Col<double> translation_;
        arma::Col<double> one_;
        arma::Mat<double> rotationR_;
        arma::Mat<double> rotationQ_;
        arma::Mat<double> deltaC101_;
        arma::Mat<double> localOptimaY101_;
        arma::Mat<double> deltaC21_;
        arma::Mat<double> localOptimaY21_;

        arma::Mat<double> getRandomDeltaC101() const ;
        arma::Mat<double> getRandomLocalOptimaY101() const ;
        arma::Mat<double> getRandomDeltaC21() const ;
        arma::Mat<double> getRandomLocalOptimaY21() const ;
        arma::Col<double> getScaling(
            const double& condition) const ;
        arma::Col<double> getScaling(
            const arma::Col<double>& condition) const ;
        arma::Col<double> getAsymmetricTransformation(
            const double& beta, const arma::Col<double>& parameter) const ;
        double getOscillationTransformation(
            const double& value) const ;
        arma::Col<double> getOscillationTransformation(
            const arma::Col<double>& parameter) const ;
        double getPenality(
            const arma::Col<double>& parameter) const ;

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
        friend class cereal::access;
        BlackBoxOptimisationBenchmark2009() = default;

        template <typename Archive>
        void serialize(
            Archive& archive)  {
          archive(cereal::make_nvp("optimisationProblem", cereal::base_class<OptimisationProblem>(this)));
          archive(cereal::make_nvp("translation", translation_));
        }
#endif
    };
  }
}
