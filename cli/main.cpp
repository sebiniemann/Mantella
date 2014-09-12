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

  std::array<unsigned int, 4> populations({10, 50, 100, 200});
  std::array<unsigned int, 6> dimensions({2, 3, 5, 10, 20, 40});
  arma::Col<double> neighbourhoodParameters = arma::linspace(0.1, 1, 10);
  arma::Col<double> accelerationParameters = arma::linspace(0, 2, 21);
  arma::Col<double> globalAttractionParameters = arma::linspace(0, 2, 21);
  arma::Col<double> localAttractionParameters = arma::linspace(0, 2, 21);

  unsigned int iterations = 15;

  for(auto dimension : dimensions) {
    std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::SphereFunction(dimension));

    for(auto population : populations) {
      hop::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, population);
      optimisationAlgorithm.setMaximalNumberOfIterations(10000);

      arma::Mat<double> results(iterations, 6 + dimension);

      for(auto localAttractionParameter : localAttractionParameters) {
      optimisationAlgorithm.setLocalAttraction(localAttractionParameter);


       for(auto globalAttractionParameter : globalAttractionParameters) {
         optimisationAlgorithm.setGlobalAttraction(globalAttractionParameter);


          for(auto accelerationParameter: accelerationParameters){
            optimisationAlgorithm.setAcceleration(accelerationParameter);


              for(auto neighbourhoodParameter : neighbourhoodParameters){
                  optimisationAlgorithm.setNeighbourhoodProbability(neighbourhoodParameter);


                    for(std::size_t n = 0; n < iterations; ++n) {
                      optimisationAlgorithm.optimise();

                      results.at(n, 0) = optimisationProblem->getNumberOfEvaluations();
                      results.at(n, 1) = optimisationProblem->getNumberOfDistinctEvaluations();
                      results.at(n, 2) = optimisationAlgorithm.getNumberOfIterations();
                      results.at(n, 3) = optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue();
                      results.at(n, 4) = optimisationAlgorithm.isFinished();
                      results.at(n, 5) = optimisationAlgorithm.isTerminated();
                      results(n, arma::span(6, 6 + dimension - 1)) = optimisationAlgorithm.getBestSolution().t();
                  }
               //TODO:: IF file not Exist create it!!!
               results.save("./Function_" + std::to_string(dimension)+ "_pop" + std::to_string(population) + "_neigh" + std::to_string(neighbourhoodParameter) + "_acc" + std::to_string(accelerationParameter) + "_local" + std::to_string(localAttractionParameter) + "_global" + std::to_string(globalAttractionParameter) +".mat", arma::raw_ascii);
              }
          }
       }
    }

    }
  }
  return EXIT_SUCCESS;
}
