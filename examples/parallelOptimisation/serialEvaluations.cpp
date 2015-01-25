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

#include <mantella>

int main (const int argc, const char* argv[]) {
  if (argc < 4) {
  }

  arma::arma_rng::set_seed_random();

  unsigned int parameterisationRow = std::stoi(argv[1]);

  unsigned int iterations = 30;

  arma::field<std::string> list;
  list.load("parameterisations.mat");

      std::shared_ptr<mant::OptimisationProblem> optimisationProblem;
      unsigned int numberOfDimensions = std::stoi(list.at(parameterisationRow, 1));

      if(list.at(parameterisationRow, 0) == "SphereFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::SphereFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "EllipsoidalFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::EllipsoidalFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "RastriginFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::RastriginFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "BuecheRastriginFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::BuecheRastriginFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "LinearSlope"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::LinearSlope(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "AttractiveSectorFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::AttractiveSectorFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "StepEllipsoidalFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::StepEllipsoidalFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "RosenbrockFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::RosenbrockFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "RosenbrockFunctionRotated"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::RosenbrockFunctionRotated(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "EllipsoidalFunctionRotated"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::EllipsoidalFunctionRotated(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "DiscusFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::DiscusFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "BentCigarFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::BentCigarFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "SharpRidgeFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::SharpRidgeFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "DifferentPowersFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::DifferentPowersFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "RastriginFunctionRotated"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::RastriginFunctionRotated(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "WeierstrassFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::WeierstrassFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "SchaffersF7Function"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::SchaffersF7Function(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "SchaffersF7FunctionIllConditioned"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::SchaffersF7FunctionIllConditioned(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "CompositeGriewankRosenbrockFunctionF8F2"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::CompositeGriewankRosenbrockFunctionF8F2(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "SchwefelFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::SchwefelFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "GallaghersGaussian21hiPeaksFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::GallaghersGaussian21hiPeaksFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "GallaghersGaussian101mePeaksFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::GallaghersGaussian101mePeaksFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "KatsuuraFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::KatsuuraFunction(numberOfDimensions));
      } else if(list.at(parameterisationRow, 0) == "LunacekBiRastriginFunction"){
          optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::LunacekBiRastriginFunction(numberOfDimensions));
      } else {
         throw std::runtime_error("Unsupported optimisation problem: " + list.at(parameterisationRow, 0));
      }

      mant::StandardParticleSwarmOptimisation2011 optimisationAlgorithm(optimisationProblem, (std::stoi(list.at(parameterisationRow, 2))));
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
