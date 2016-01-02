#pragma once

// Mantella
#include "mantella_bits/optimisationProblem/surrogateModel.hpp"

namespace mant {
  class MultivariateAdaptiveRegressionSplinesModel : public SurrogateModel {
   public:
    using SurrogateModel::SurrogateModel;
  };
}
