#include <cstdlib>
#include <string>
#include <map>
#include <tuple>
#include <iostream>
#include <stdexcept>

#include <armadillo>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <boost/filesystem.hpp>

#include <hop>

int main (const int argc, const char* argv[]) {
  if (argc < 4) {
  }

  arma::arma_rng::set_seed_random();

  unsigned int parameterisationRow = std::stoi(argv[1]);

  unsigned int iterations = 30;

  arma::field<std::string> list;
  list.load("parameterisations.mat");

      std::shared_ptr<hop::OptimisationProblem> optimisationProblem;
      unsigned int numberOfDimensions = std::stoi(list.at(parameterisationRow, 1));

      if(list.at(parameterisationRow, 0) == "SphereFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::SphereFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "EllipsoidalFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::EllipsoidalFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "RastriginFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::RastriginFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "BuecheRastriginFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::BuecheRastriginFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "LinearSlope"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::LinearSlope(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "AttractiveSectorFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::AttractiveSectorFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "StepEllipsoidalFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::StepEllipsoidalFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "RosenbrockFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::RosenbrockFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "RosenbrockFunctionRotated"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::RosenbrockFunctionRotated(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "EllipsoidalFunctionRotated"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::EllipsoidalFunctionRotated(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "DiscusFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::DiscusFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "BentCigarFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::BentCigarFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "SharpRidgeFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::SharpRidgeFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "DifferentPowersFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::DifferentPowersFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "RastriginFunctionRotated"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::RastriginFunctionRotated(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "WeierstrassFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::WeierstrassFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "SchaffersF7Function"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::SchaffersF7Function(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "SchaffersF7FunctionIllConditioned"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::SchaffersF7FunctionIllConditioned(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "CompositeGriewankRosenbrockFunctionF8F2"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::CompositeGriewankRosenbrockFunctionF8F2(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "SchwefelFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::SchwefelFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "GallaghersGaussian21hiPeaksFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::GallaghersGaussian21hiPeaksFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "GallaghersGaussian101mePeaksFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::GallaghersGaussian101mePeaksFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "KatsuuraFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::KatsuuraFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "LunacekBiRastriginFunction"){
          optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::LunacekBiRastriginFunction(numberOfDimensions));
      } else {
         throw std::runtime_error("Unsupported optimisation problem: " + list.at(parameterisationRow, 0));
      }

      hop::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, (std::stoi(list.at(parameterisationRow, 2))));
      optimisationAlgorithm.setMaximalNumberOfIterations(10000);
      optimisationAlgorithm.setLocalAttraction(std::stod(list.at(parameterisationRow, 3)));
      optimisationAlgorithm.setGlobalAttraction(std::stod(list.at(parameterisationRow, 4)));
      optimisationAlgorithm.setAcceleration(std::stod(list.at(parameterisationRow, 5)));
      optimisationAlgorithm.setNeighbourhoodProbability(std::stod(list.at(parameterisationRow, 6)));

          arma::Col<arma::uword> totalNumberOfIterations(iterations);
          arma::Col<double> temp(iterations);

          for(std::size_t n = 0; n < iterations; ++n) {
            optimisationAlgorithm.optimise();

            totalNumberOfIterations.at(n) = optimisationProblem->getNumberOfDistinctEvaluations();

            if (optimisationAlgorithm.isFinished()){
             temp.at(n) = optimisationProblem->getNumberOfDistinctEvaluations();
           } else {
              temp.at(n) = 0;
            }

          }
          arma::Col<double> numberOfItreations = temp.rows(arma::find(temp > 0));

          totalNumberOfIterations.save("./total/totalSerialNumberOfIterations_" + list.at(parameterisationRow,0) + "_dim" + list.at(parameterisationRow, 1) + "_pop" + list.at(parameterisationRow, 2) + ".mat", arma::raw_ascii);
          numberOfItreations.save("./serialNumberOfIterations_" + list.at(parameterisationRow,0) + "_dim" + list.at(parameterisationRow, 1) + "_pop" + list.at(parameterisationRow, 2) + ".mat", arma::raw_ascii);

  return EXIT_SUCCESS;
}
