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
  std::cout << "isLinear: " << propertiesEstimation.isLinear(0) << std::endl;
  std::cout << "getLinearCharacterisation: " << propertiesEstimation.getLinearEstimator(0) << std::endl;

  return EXIT_SUCCESS;
}
