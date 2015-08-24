#include <mantella_bits/propertyAnalysis/activePropertyAnalysis/polynomialProportionalityAnalysis.hpp>

namespace mant {
  PolynomialProportionalityAnalysis::PolynomialProportionalityAnalysis(
      std::shared_ptr<OptimisationProblem> optimisationProblem,
      const std::shared_ptr<RegressionFunction> regressionFunction) 
    : ActivePropertyAnalysis(optimisationProblem),
      regressionFunction_(regressionFunction) {
      
  }
          
  void PolynomialProportionalityAnalysis::analyseImplementation() {
  
  }

  std::string PolynomialProportionalityAnalysis::toString() const {
    return "polynomial_proportionality_analysis";
  }
}
