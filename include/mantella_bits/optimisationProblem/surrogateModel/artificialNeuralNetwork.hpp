#pragma once

// Mantella
#include "mantella_bits/optimisationProblem/surrogateModel.hpp"

namespace mant {
  class ArtificialNeuralNetwork : public SurrogateModel {
   public:
    using SurrogateModel::SurrogateModel;
  };
}
