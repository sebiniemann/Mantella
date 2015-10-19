#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
namespace arma {
  template <typename eT>
  class Col;
}

// Mantella
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/helper/unorderedContainer.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual

namespace mant {
  class SurrogateModel : public OptimisationProblem {
   public:
    explicit SurrogateModel(
        const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);

    void model();

    virtual ~SurrogateModel() = default;

   protected:
    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples_;

    virtual void modelImplementation() = 0;
  };
}
