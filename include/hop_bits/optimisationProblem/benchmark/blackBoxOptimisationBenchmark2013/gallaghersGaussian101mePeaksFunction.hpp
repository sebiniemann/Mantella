#pragma once

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark2013 {
    public:
      explicit GallaghersGaussian101mePeaksFunction(const unsigned int& numberOfDimensions);

      GallaghersGaussian101mePeaksFunction(const GallaghersGaussian101mePeaksFunction&) = delete;
      GallaghersGaussian101mePeaksFunction& operator=(const GallaghersGaussian101mePeaksFunction&) = delete;

      std::string to_string() const override;

    protected:
      arma::Col<double> weight_;

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        archive(cereal::make_nvp("rotationR", rotationR_));
        archive(cereal::make_nvp("deltaC101", deltaC101_));
        archive(cereal::make_nvp("localOptimaY101", localOptimaY101_));
        archive(cereal::make_nvp("weight", weight_));
      }

      template<class T>
      static void load_and_construct(T& archive, cereal::construct<GallaghersGaussian101mePeaksFunction>& construct) {
        unsigned int numberOfDimensions;
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
        construct(numberOfDimensions);

        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
        archive(cereal::make_nvp("rotationR", construct->rotationR_));
        archive(cereal::make_nvp("deltaC101", construct->deltaC101_));
        archive(cereal::make_nvp("localOptimaY101", construct->localOptimaY101_));
        archive(cereal::make_nvp("weight", construct->weight_));
      }
  };
}
