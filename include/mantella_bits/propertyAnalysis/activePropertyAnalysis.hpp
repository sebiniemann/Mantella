#pragma once

// C++ standard library
#include <memory>

// Mantella
#include <mantella_bits/propertyAnalysis.hpp>
#include <mantella_bits/optimisationProblem.hpp>

namespace mant {
  class ActivePropertyAnalysis : public PropertyAnalysis {
    public:
      explicit ActivePropertyAnalysis(
          std::shared_ptr<OptimisationProblem> optimisationProblem);
      
      void analyse();

    protected:
      std::shared_ptr<OptimisationProblem> optimisationProblem_;
    
      virtual void analyseImplementation() = 0;
  };
}
