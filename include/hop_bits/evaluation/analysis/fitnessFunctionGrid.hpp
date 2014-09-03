#pragma once

#include <memory>

#include <cereal/types/memory.hpp>
#include <cereal/access.hpp>

#include <hop>

#include <experiment.hpp>

class FitnessFunctionGrid : public Experiment {
  public:
    FitnessFunctionGrid(const std::shared_ptr<hop::OptimisationProblem> optimisationProblem);

  protected:
    std::shared_ptr<hop::OptimisationProblem> _optimisationProblem;

    void runImpelementation();

  private:
    friend class cereal::access;

    FitnessFunctionGrid() = default;

    template<class T>
    void serialize(T& archive) {
      archive(CEREAL_NVP(_optimisationProblem));
    }
};
