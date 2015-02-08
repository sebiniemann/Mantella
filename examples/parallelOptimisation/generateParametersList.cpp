#include <cstdlib>
#include <string>
#include <map>
#include <tuple>
#include <iostream>

#include <armadillo>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <boost/filesystem.hpp>

#include <mantella>

int main (const int argc, const char* argv[]) {
  if (argc < 4) {

  }

  arma::arma_rng::set_seed_random();

  unsigned int optimisationProblemIndex = std::stoi(argv[1]);
  unsigned int numberOfDimensions = std::stoi(argv[2]);
  unsigned int populationSize = std::stoi(argv[3]);

  std::array<std::shared_ptr<mant::OptimisationProblem>, 24> optimisationProblems({
    std::shared_ptr<mant::OptimisationProblem>(new mant::SphereFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::EllipsoidalFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::RastriginFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::BuecheRastriginFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::LinearSlope(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::AttractiveSectorFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::StepEllipsoidalFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::RosenbrockFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::RosenbrockFunctionRotated(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::EllipsoidalFunctionRotated(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::DiscusFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::BentCigarFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::SharpRidgeFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::DifferentPowersFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::RastriginFunctionRotated(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::WeierstrassFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::SchaffersF7Function(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::SchaffersF7FunctionIllConditioned(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::CompositeGriewankRosenbrockFunctionF8F2(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::SchwefelFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::GallaghersGaussian101mePeaksFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::GallaghersGaussian21hiPeaksFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::KatsuuraFunction(numberOfDimensions)),
    std::shared_ptr<mant::OptimisationProblem>(new mant::LunacekBiRastriginFunction(numberOfDimensions)),
  });

  std::shared_ptr<mant::OptimisationProblem> optimisationProblem = optimisationProblems.at(optimisationProblemIndex);

  arma::Col<double> localAttractionParameters = arma::linspace(0, 2, 10);
  arma::Col<double> globalAttractionParameters = arma::linspace(0, 2, 10);
  arma::Col<double> accelerationParameters = arma::linspace(0, 2, 10);
  arma::Col<double> neighbourhoodParameters = arma::linspace(0.1, 1, 5);

  mant::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, populationSize);
  optimisationAlgorithm.setMaximalNumberOfIterations(10000);

  std::fstream parameterisationsFile;
  parameterisationsFile.open("./parameterisations.mat", std::fstream::out | std::fstream::app);

  std::map<std::tuple<std::string, unsigned int, unsigned int>, arma::Col<double>> parameterisations;

  arma::Mat<double> results(localAttractionParameters.n_elem * globalAttractionParameters.n_elem * accelerationParameters.n_elem * neighbourhoodParameters.n_elem,  7);
  size_t n = 0;
  for(auto localAttractionParameter : localAttractionParameters) {
    optimisationAlgorithm.setLocalAttraction(localAttractionParameter);

    for(auto globalAttractionParameter : globalAttractionParameters) {
      optimisationAlgorithm.setGlobalAttraction(globalAttractionParameter);

      for(auto accelerationParameter: accelerationParameters) {
        optimisationAlgorithm.setAcceleration(accelerationParameter);

        for(auto neighbourhoodParameter : neighbourhoodParameters) {
          optimisationAlgorithm.setNeighbourhoodProbability(neighbourhoodParameter);

          boost::filesystem::path filepath = boost::filesystem::path("/data/shuka/OnlineOptimisation/bin/parameters/parameterisation_prob" + mant::to_string(optimisationProblem) + "_dim" + std::to_string(numberOfDimensions)+ "_pop" + std::to_string(populationSize) + "_local" + std::to_string(localAttractionParameter) + "_global" + std::to_string(globalAttractionParameter) + "_acc" + std::to_string(accelerationParameter) + "_neigh" + std::to_string(neighbourhoodParameter) + ".mat");

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

  double isAnyoneFinished = static_cast<arma::Col<double>>(results.col(2)).max();
  arma::Mat<double> maximisedIsFinished = results.rows(arma::find(results.col(2) >= isAnyoneFinished));

  arma::Row<double> bestParameteristation;
  if(isAnyoneFinished > 0) {
    arma::Mat<double> minisedNumberOfDistinctEvaluations = maximisedIsFinished.rows(arma::find(maximisedIsFinished.col(0) <= static_cast<arma::Col<double>>(maximisedIsFinished.col(0)).min()));

    arma::uword bestParameteristationIndex;
    static_cast<arma::Col<double>>(minisedNumberOfDistinctEvaluations.col(1)).min(bestParameteristationIndex);
    bestParameteristation = minisedNumberOfDistinctEvaluations(bestParameteristationIndex, arma::span(3, 6));
  } else {
    arma::Mat<double> minisedBestObjectiveValueDifference = maximisedIsFinished.rows(arma::find(maximisedIsFinished.col(1) <= static_cast<arma::Col<double>>(maximisedIsFinished.col(1)).min()));

    arma::uword bestParameteristationIndex;
    static_cast<arma::Col<double>>(minisedBestObjectiveValueDifference.col(0)).min(bestParameteristationIndex);
    bestParameteristation = minisedBestObjectiveValueDifference(bestParameteristationIndex, arma::span(3, 6));
  }

  parameterisations.insert({std::make_tuple(mant::to_string(optimisationProblem), numberOfDimensions, populationSize), bestParameteristation.t()});
  parameterisationsFile << mant::to_string(optimisationProblem) << " " << numberOfDimensions << " " << populationSize << " " << bestParameteristation.at(0) << " " << bestParameteristation.at(1) << " " << bestParameteristation.at(2) << " " << bestParameteristation.at(3) << std::endl;

  parameterisationsFile.close();

  return EXIT_SUCCESS;
}