#include <cstdlib>
#include <string>
#include <map>
#include <tuple>
#include <iostream>
#include <sstream>

#include <armadillo>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <boost/filesystem.hpp>

#include <hop>

int main (const int argc, const char* argv[]) {
  arma::arma_rng::set_seed_random();

  // std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::LunacekBiRastriginFunction(2));
  // std::shared_ptr<hop::Evaluation> evaluation(new hop::FitnessFunctionGrid(optimisationProblem));

  // std::ofstream output(evaluation->getEvaluationPath() + "/evaluation.serialised"); {
    // cereal::JSONOutputArchive archive(output);
    // archive(cereal::make_nvp("evaluation", evaluation));
  // } output.close();
  
  unsigned int numberOfDimensions = std::stoi(argv[1]);
  unsigned int populationSize = std::stoi(argv[2]);
  unsigned int optimisationProblemIndex = std::stoi(argv[3]);

  std::array<std::shared_ptr<hop::OptimisationProblem>, 24> optimisationProblems({
    std::shared_ptr<hop::OptimisationProblem>(new hop::SphereFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::EllipsoidalFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::RastriginFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::BuecheRastriginFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::LinearSlope(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::AttractiveSectorFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::StepEllipsoidalFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::RosenbrockFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::RosenbrockFunctionRotated(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::EllipsoidalFunctionRotated(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::DiscusFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::BentCigarFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::SharpRidgeFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::DifferentPowersFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::RastriginFunctionRotated(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::WeierstrassFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::SchaffersF7Function(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::SchaffersF7FunctionIllConditioned(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::CompositeGriewankRosenbrockFunctionF8F2(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::SchwefelFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::GallaghersGaussian101mePeaksFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::GallaghersGaussian21hiPeaksFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::KatsuuraFunction(numberOfDimensions)),
    std::shared_ptr<hop::OptimisationProblem>(new hop::LunacekBiRastriginFunction(numberOfDimensions)),
  });

  std::shared_ptr<hop::OptimisationProblem> optimisationProblem = optimisationProblems.at(optimisationProblemIndex);

  arma::Col<double> localAttractionParameters = arma::linspace(0, 2, 10);
  arma::Col<double> globalAttractionParameters = arma::linspace(0, 2, 10);
  arma::Col<double> accelerationParameters = arma::linspace(0, 2, 10);
  arma::Col<double> neighbourhoodParameters = arma::linspace(0.1, 1, 5);

  unsigned int iterations = 10;

  hop::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, populationSize);
  optimisationAlgorithm.setMaximalNumberOfIterations(10000);

  for(auto localAttractionParameter : localAttractionParameters) {
    optimisationAlgorithm.setLocalAttraction(localAttractionParameter);

    for(auto globalAttractionParameter : globalAttractionParameters) {
      optimisationAlgorithm.setGlobalAttraction(globalAttractionParameter);

      for(auto accelerationParameter: accelerationParameters) {
        optimisationAlgorithm.setAcceleration(accelerationParameter);

        for(auto neighbourhoodParameter : neighbourhoodParameters) {
          optimisationAlgorithm.setNeighbourhoodProbability(neighbourhoodParameter);

          boost::filesystem::path filepath = boost::filesystem::path("./parameterisation_prob" + hop::to_string(optimisationProblem) + "_dim" + std::to_string(numberOfDimensions)+ "_pop" + std::to_string(populationSize) + "_local" + std::to_string(localAttractionParameter) + "_global" + std::to_string(globalAttractionParameter) + "_acc" + std::to_string(accelerationParameter) + "_neigh" + std::to_string(neighbourhoodParameter) + ".mat");

          if(!boost::filesystem::exists(filepath)) {
            arma::Mat<double> results(iterations, 6 + numberOfDimensions);
            for(std::size_t n = 0; n < iterations; ++n) {
              optimisationAlgorithm.optimise();

              results.at(n, 0) = optimisationProblem->getNumberOfEvaluations();
              results.at(n, 1) = optimisationProblem->getNumberOfDistinctEvaluations();
              results.at(n, 2) = optimisationAlgorithm.getNumberOfIterations();
              results.at(n, 3) = optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue();
              results.at(n, 4) = optimisationAlgorithm.isFinished();
              results.at(n, 5) = optimisationAlgorithm.isTerminated();
              results(n, arma::span(6, 6 + numberOfDimensions - 1)) = optimisationAlgorithm.getBestSolution().t();
            }

            results.save(filepath.string(), arma::raw_ascii);
          }
        }
      }
    }
  }

  std::ofstream parameterisationsFile("./parameterisations.mat");
  std::map<std::tuple<std::string, unsigned int, unsigned int>, arma::Col<double>> parameterisations;

  arma::Mat<double> results(localAttractionParameters.n_elem * globalAttractionParameters.n_elem * accelerationParameters.n_elem * neighbourhoodParameters.n_elem,  7);
  size_t n = 0;
  for(auto localAttractionParameter : localAttractionParameters) {
    optimisationAlgorithm.setLocalAttraction(localAttractionParameter);

    for(auto globalAttractionParameter : globalAttractionParameters) {
      optimisationAlgorithm.setGlobalAttraction(globalAttractionParameter);

      for(auto accelerationParameter : accelerationParameters) {
        optimisationAlgorithm.setAcceleration(accelerationParameter);

        for(auto neighbourhoodParameter : neighbourhoodParameters) {
          optimisationAlgorithm.setNeighbourhoodProbability(neighbourhoodParameter);

          boost::filesystem::path filepath = boost::filesystem::path("./parameterisation_prob" + hop::to_string(optimisationProblem) + "_dim" + std::to_string(numberOfDimensions)+ "_pop" + std::to_string(populationSize) + "_local" + std::to_string(localAttractionParameter) + "_global" + std::to_string(globalAttractionParameter) + "_acc" + std::to_string(accelerationParameter) + "_neigh" + std::to_string(neighbourhoodParameter) + ".mat");

          if(!boost::filesystem::exists(filepath)) {
            // TODO Add exception
          }

          arma::Mat<double> result;
          result.load(filepath.string(), arma::raw_ascii);

          results.at(n, 0) = arma::mean(result.col(1));
          results.at(n, 1) = arma::mean(result.col(3));
          results.at(n, 2) = arma::mean(result.col(4));
          results(n, arma::span(3, 6)) = arma::Row<double>({localAttractionParameter, globalAttractionParameter, accelerationParameter, neighbourhoodParameter});
          ++n;
        }
      }
    }
  }

  arma::Mat<double> maximisedIsFinished = results.rows(arma::find(results.col(2) >= static_cast<arma::Col<double>>(results.col(2)).max()));
  arma::Mat<double> minisedNumberOfDistinctEvaluations = maximisedIsFinished.rows(arma::find(maximisedIsFinished.col(0) <= static_cast<arma::Col<double>>(maximisedIsFinished.col(0)).min()));

  arma::uword bestParameteristationIndex;
  static_cast<arma::Col<double>>(minisedNumberOfDistinctEvaluations.col(1)).min(bestParameteristationIndex);
  arma::Row<double> bestParameteristation = minisedNumberOfDistinctEvaluations(bestParameteristationIndex, arma::span(3, 6));

  parameterisations.insert({std::make_tuple(hop::to_string(optimisationProblem), numberOfDimensions, populationSize), bestParameteristation.t()});
  parameterisationsFile << hop::to_string(optimisationProblem) << "," << numberOfDimensions << "," << populationSize << "," << bestParameteristation.at(0) << "," << bestParameteristation.at(1) << "," << bestParameteristation.at(2) << "," << bestParameteristation.at(3) << std::endl;

  parameterisationsFile.close();

  return EXIT_SUCCESS;
}
