#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/unorderedContainer.hpp>
#include <mantella_bits/optimisationProblem.hpp>

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
