#include <cstdlib>
#include <string>

#include <armadillo>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <hop>

int main (const int argc, const char* argv[]) {
  if (argc < 2) {

  }

  arma::arma_rng::set_seed_random();

  std::array<unsigned int, 3> populations({10, 50, 100});
  std::array<unsigned int, 5> communicationSteps({1, 5, 10, 50, 100});
  std::array<unsigned int, 4> dimensions({5, 10, 20, 40});

  unsigned int iterations = 15;

  for(auto dimension : dimensions) {
    std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::SphereFunction(dimension));
    optimisationProblem->setMaximalNumberOfEvaluations(2000);

    for(auto population : populations) {
      hop::ParallelStandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, population);

      for(auto communicationStep : communicationSteps) {
        optimisationAlgorithm.setCommunicationSteps(communicationStep);

        arma::Col<arma::uword> numberOfIterations(iterations);

        for(std::size_t n = 0; n < iterations; ++n) {
          optimisationAlgorithm.optimise();

          std::cout << "isFinished: " << optimisationAlgorithm.isFinished() << std::endl;
          std::cout << "isTerminated: " << optimisationAlgorithm.isTerminated() << std::endl;
          std::cout << "bestObjectiveValue: " << optimisationAlgorithm.getBestObjectiveValue() << std::endl;
          std::cout << "acceptableObjectiveValue: " << optimisationProblem->getAcceptableObjectiveValue() << std::endl;
          std::cout << "bestSolution: " << optimisationAlgorithm.getBestSolution() << std::endl;
          std::cout << "numberOfIterations: " << optimisationAlgorithm.getNumberOfIterations() << std::endl;
          std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;

          numberOfIterations.at(n) = optimisationAlgorithm.getNumberOfIterations();
        }
        numberOfIterations.save("./numberOfIterations_f1_" + std::to_string(communicationStep) + "_" + std::to_string(population) + "_" + std::to_string(dimension) + ".mat", arma::raw_ascii);
      }
    }
  }
  return EXIT_SUCCESS;
}
