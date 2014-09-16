#include <cstdlib>
#include <string>
#include <map>
#include <tuple>
#include <iostream>

#include <armadillo>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <boost/filesystem.hpp>

#include <hop>

int main (const int argc, const char* argv[]) {
  if (argc < 2) {

  }

  arma::arma_rng::set_seed_random();

  std::array<unsigned int, 4> populations({10, 50, 100, 200});
  std::array<unsigned int, 6> dimensions({2, 3, 5, 10, 20, 40});
  arma::Col<double> localAttractionParameters = arma::linspace(0, 2, 10);
  arma::Col<double> globalAttractionParameters = arma::linspace(0, 2, 10);
  arma::Col<double> accelerationParameters = arma::linspace(0, 2, 10);
  arma::Col<double> neighbourhoodParameters = arma::linspace(0.1, 1, 5);

  unsigned int iterations = 10;

  for(auto dimension : dimensions) {  
    std::array<std::shared_ptr<hop::OptimisationProblem>, 24> optimisationProblems({
      std::shared_ptr<hop::OptimisationProblem>(new hop::SphereFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::EllipsoidalFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::RastriginFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::BuecheRastriginFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::LinearSlope(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::AttractiveSectorFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::StepEllipsoidalFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::RosenbrockFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::RosenbrockFunctionRotated(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::EllipsoidalFunctionRotated(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::DiscusFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::BentCigarFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::SharpRidgeFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::DifferentPowersFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::RastriginFunctionRotated(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::WeierstrassFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::SchaffersF7Function(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::SchaffersF7FunctionIllConditioned(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::CompositeGriewankRosenbrockFunctionF8F2(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::SchwefelFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::GallaghersGaussian101mePeaksFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::GallaghersGaussian21hiPeaksFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::KatsuuraFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::LunacekBiRastriginFunction(dimension)),
    });

    for(auto optimisationProblem : optimisationProblems) {
      for(auto population : populations) {
        hop::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, population);
        optimisationAlgorithm.setMaximalNumberOfIterations(10000);

        for(auto localAttractionParameter : localAttractionParameters) {
          optimisationAlgorithm.setLocalAttraction(localAttractionParameter);

          for(auto globalAttractionParameter : globalAttractionParameters) {
            optimisationAlgorithm.setGlobalAttraction(globalAttractionParameter);

            for(auto accelerationParameter: accelerationParameters) {
              optimisationAlgorithm.setAcceleration(accelerationParameter);

              for(auto neighbourhoodParameter : neighbourhoodParameters) {
                optimisationAlgorithm.setNeighbourhoodProbability(neighbourhoodParameter);

                boost::filesystem::path filepath = boost::filesystem::path("./parameterisation_prob" + hop::to_string(optimisationProblem) + "_dim" + std::to_string(dimension)+ "_pop" + std::to_string(population) + "_neigh" + std::to_string(neighbourhoodParameter) + "_acc" + std::to_string(accelerationParameter) + "_local" + std::to_string(localAttractionParameter) + "_global" + std::to_string(globalAttractionParameter) + ".mat");

                if(!boost::filesystem::exists(filepath)) {
                  arma::Mat<double> results(iterations, 6 + dimension);
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

                  results.save(filepath.string(), arma::raw_ascii);
                }
              }
            }
          }
        }
      }
    }
  }

  std::ofstream parameterisationsFile("./parameterisations.mat");
  std::map<std::tuple<std::string, unsigned int, unsigned int>, arma::Col<double>> parameterisations;
  for(auto dimension : dimensions) {
    std::array<std::shared_ptr<hop::OptimisationProblem>, 24> optimisationProblems({
      std::shared_ptr<hop::OptimisationProblem>(new hop::SphereFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::EllipsoidalFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::RastriginFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::BuecheRastriginFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::LinearSlope(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::AttractiveSectorFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::StepEllipsoidalFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::RosenbrockFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::RosenbrockFunctionRotated(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::EllipsoidalFunctionRotated(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::DiscusFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::BentCigarFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::SharpRidgeFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::DifferentPowersFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::RastriginFunctionRotated(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::WeierstrassFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::SchaffersF7Function(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::SchaffersF7FunctionIllConditioned(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::CompositeGriewankRosenbrockFunctionF8F2(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::SchwefelFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::GallaghersGaussian101mePeaksFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::GallaghersGaussian21hiPeaksFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::KatsuuraFunction(dimension)),
      std::shared_ptr<hop::OptimisationProblem>(new hop::LunacekBiRastriginFunction(dimension)),
    });

    for(auto optimisationProblem : optimisationProblems) {
      for(auto population : populations) {
        hop::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, population);
        optimisationAlgorithm.setMaximalNumberOfIterations(10000);


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

                boost::filesystem::path filepath = boost::filesystem::path("./parameterisation_prob" + hop::to_string(optimisationProblem) + "_dim" + std::to_string(dimension)+ "_pop" + std::to_string(population) + "_neigh" + std::to_string(neighbourhoodParameter) + "_acc" + std::to_string(accelerationParameter) + "_local" + std::to_string(localAttractionParameter) + "_global" + std::to_string(globalAttractionParameter) + ".mat");

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

        parameterisations.insert({std::make_tuple(hop::to_string(optimisationProblem), dimension, population), bestParameteristation.t()});
        parameterisationsFile << hop::to_string(optimisationProblem) << "," << dimension << "," << population << "," << bestParameteristation.at(0) << "," << bestParameteristation.at(1) << "," << bestParameteristation.at(2) << "," << bestParameteristation.at(3) << std::endl;
      }
    }
  }
  parameterisationsFile.close();

  return EXIT_SUCCESS;
}
