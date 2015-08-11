#include <mantella_bits/propertyAnalysis/activePropertyAnalysis.hpp>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {
  ActivePropertyAnalysis::ActivePropertyAnalysis(
      std::shared_ptr<OptimisationProblem> optimisationProblem) 
    : optimisationProblem_(optimisationProblem) {
    
  }
  
  void ActivePropertyAnalysis::analyse() {
    verify(arma::all(optimisationProblem_->getLowerBounds() <= optimisationProblem_->getUpperBounds()), "All upper bounds of the optimisation problem must be greater than or equal to its lower bound.");
    
    analyseImplementation();
  }
}
