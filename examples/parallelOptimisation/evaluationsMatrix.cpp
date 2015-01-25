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
  std::array<unsigned int, 9> population({10, 15, 20, 25, 30, 35, 40, 45, 50});
  std::array<unsigned int, 10> communicationSteps({1, 2, 3, 4, 5, 10, 15, 20, 25, 30});

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
  arma::Mat<double> matrixMean(communicationSteps.size(), population.size());
  arma::Mat<double> matrixStddev(communicationSteps.size(), population.size());
  arma::Mat<double> matrixMin(communicationSteps.size(), population.size());
  arma::Mat<double> matrixMax(communicationSteps.size(), population.size());
  arma::Mat<double> matrixMed(communicationSteps.size(), population.size());
  arma::Mat<double> matrixSucc(communicationSteps.size(), population.size());

  std::size_t rowNumber = 0;
  for(auto communicationStep : communicationSteps) {

  std::size_t colNumber = 0;
    for(auto populationSize : population) {

      boost::filesystem::path filepath = boost::filesystem::path("/homes/shuka/Desktop/OnlineOptimisation/bin/Final/Evaluation/parallelNumberOfIterations_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions) + "_pop" + std::to_string(populationSize) + "_step" + std::to_string(communicationStep) + ".mat");

      if(!boost::filesystem::exists(filepath)) {
            // TODO Add exception
      }

      arma::Mat<double> temp;
     temp.load("/homes/shuka/Desktop/OnlineOptimisation/bin/Final/Evaluation/parallelNumberOfIterations_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions) + "_pop" + std::to_string(populationSize) + "_step" + std::to_string(communicationStep) + ".mat");

     if(!temp.is_empty()){
       matrixMean.at(rowNumber, colNumber) = arma::mean(temp.col(0));
       matrixStddev.at(rowNumber, colNumber) = arma::stddev(temp.col(0));
       matrixMin.at(rowNumber, colNumber) = arma::min(temp.col(0));
       matrixMax.at(rowNumber, colNumber) = arma::max(temp.col(0));
       matrixMed.at(rowNumber, colNumber) = arma::median(temp.col(0));
       matrixSucc.at(rowNumber, colNumber) = (temp.col(0).n_rows/30.0) * 100;
     } else {
       matrixMean.at(rowNumber, colNumber) = arma::datum::nan;
       matrixStddev.at(rowNumber, colNumber) = arma::datum::nan;
       matrixMin.at(rowNumber, colNumber) = arma::datum::nan;
       matrixMax.at(rowNumber, colNumber) = arma::datum::nan;
       matrixMed.at(rowNumber, colNumber) = arma::datum::nan;
       matrixSucc.at(rowNumber, colNumber) = arma::datum::nan;
     }
      ++colNumber;
    }

  ++rowNumber;
  }

  matrixMean.save("./Evaluations_Mean_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixStddev.save("./Evaluations_Stddev_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixMin.save("./Evaluations_Min_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixMax.save("./Evaluations_Max_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixMed.save("./Evaluations_Median_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixSucc.save("./Evaluations_Success_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  return EXIT_SUCCESS;
}
