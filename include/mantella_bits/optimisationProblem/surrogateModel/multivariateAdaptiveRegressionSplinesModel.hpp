#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/surrogateModel.hpp"

namespace mant {
  class MultivariateAdaptiveRegressionSplinesModel : public SurrogateModel {
   public:
    using SurrogateModel::SurrogateModel;
  };
}
