#pragma once

#include <memory>

#include <armadillo>

#include <hop_bits/evaluation.hpp>
#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class FitnessFunctionGrid : public Evaluation {
    public:
      explicit FitnessFunctionGrid(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      FitnessFunctionGrid(const FitnessFunctionGrid&) = delete;
      FitnessFunctionGrid& operator=(const FitnessFunctionGrid&) = delete;

      void setLowerBounds(const arma::Col<double>& lowerBounds);
      void setUpperBounds(const arma::Col<double>& upperBounds);
      void setGridSize(const arma::Col<arma::uword>& gridSize);

    protected:
      std::shared_ptr<OptimisationProblem> optimisationProblem_;
      arma::Col<double> lowerBounds_;
      arma::Col<double> upperBounds_;
      arma::Col<arma::uword> gridSize_;

      void runImpelementation() override;

      friend class cereal::access;
      FitnessFunctionGrid() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("evaluation", cereal::base_class<Evaluation>(this)));
        archive(cereal::make_nvp("optimisationProblem", optimisationProblem_));
        archive(cereal::make_nvp("lowerBounds", lowerBounds_));
        archive(cereal::make_nvp("upperBounds", upperBounds_));
        archive(cereal::make_nvp("gridSize", gridSize_));
      }
  };
}
