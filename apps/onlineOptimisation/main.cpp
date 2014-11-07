#include <cstdlib>
#include <memory>
#include <iostream>

#include <hop>

int main(int argc, char* argv[]) {
  std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::bbob2013::SphereFunction(4));

  hop::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, 10);
  optimisationAlgorithm.optimise();

  std::cout << "isFinished: " << optimisationAlgorithm.isFinished() << std::endl;
  std::cout << "isTerminated: " << optimisationAlgorithm.isTerminated() << std::endl;
  std::cout << "numberOfIterations: " << optimisationAlgorithm.getNumberOfIterations() << std::endl;
  std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
  std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
  std::cout << "bestObjectiveValueDelta: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
  std::cout << "bestSolution: " << optimisationAlgorithm.getBestSolution() << std::endl;

  hop::MaximumLikelihoodEstimation propertiesEstimation;
  propertiesEstimation.estimate(optimisationProblem);
  std::cout << "Estimated propertySets: " << propertiesEstimation.getNumberOfPropertySets() << std::endl;

  for(std::size_t n = 0; n < propertiesEstimation.getNumberOfPropertySets(); ++n) {
    std::cout << "isLinear: " << propertiesEstimation.isLinear(n) << std::endl;
    std::cout << "getLinearEstimator: " << propertiesEstimation.getLinearEstimator(n) << std::endl;
    std::cout << "isQuadratic: " << propertiesEstimation.isQuadratic(n) << std::endl;
    std::cout << "getQuadraticEstimator: " << propertiesEstimation.getQuadraticEstimator(n) << std::endl;
    std::cout << "isLipschitzContinuous: " << propertiesEstimation.isLipschitzContinuous(n) << std::endl;
    std::cout << "getLipschitzConstant: " << propertiesEstimation.getLipschitzConstant(n) << std::endl;
  }

  return EXIT_SUCCESS;
}
