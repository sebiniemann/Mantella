#include <cstdlib>
#include <memory>
#include <fstream>
#include <iostream>

#include <armadillo>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <hop>

int main (const int argc, const char* argv[]) {
  if (argc < 2) {

  }

  arma::arma_rng::set_seed_random();

  std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::SphereFunction(5));
  optimisationProblem->setMaximalNumberOfEvaluations(400);

  hop::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, 2);
  optimisationAlgorithm.optimise();

  std::cout << "isFinished: " << optimisationAlgorithm.isFinished() << std::endl;
  std::cout << "isTerminated: " << optimisationAlgorithm.isTerminated() << std::endl;
  std::cout << "bestObjectiveValue: " << optimisationAlgorithm.getBestObjectiveValue() << std::endl;
  std::cout << "acceptableObjectiveValue: " << optimisationProblem->getAcceptableObjectiveValue() << std::endl;
  std::cout << "bestSolution: " << optimisationAlgorithm.getBestSolution() << std::endl;
  std::cout << "numberOfIterations: " << optimisationAlgorithm.getNumberOfIterations() << std::endl;
  std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;

  return EXIT_SUCCESS;
}
