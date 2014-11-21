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
  if (argc < 4) {

  }

  arma::arma_rng::set_seed_random();

  unsigned int optimisationProblemIndex = std::stoi(argv[1]);
  unsigned int numberOfDimensions = std::stoi(argv[2]);
  std::array<unsigned int, 9> population({10, 15, 20, 25, 30, 35 ,40, 45, 50});

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
  arma::Mat<double> matrixMean(1, population.size());
  arma::Mat<double> matrixStddev(1, population.size());
  arma::Mat<double> matrixMin(1, population.size());
  arma::Mat<double> matrixMax(1, population.size());
  arma::Mat<double> matrixMed(1, population.size());
  arma::Mat<double> matrixSucc(1, population.size());


  std::size_t colNumber = 0;
    for(auto populationSize : population) {

      boost::filesystem::path filepath = boost::filesystem::path("/homes/shuka/Desktop/OnlineOptimisation/bin/Final/Evaluation/serialNumberOfIterations_" + hop::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions) + "_pop" + std::to_string(populationSize) + ".mat");

      if(!boost::filesystem::exists(filepath)) {
            // TODO Add exception
      }

      arma::Mat<double> temp;
      temp.load("/homes/shuka/Desktop/OnlineOptimisation/bin/Final/Evaluation/serialNumberOfIterations_" + hop::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions) + "_pop" + std::to_string(populationSize) + ".mat");
      if(!temp.is_empty()){
        matrixMean.at(0, colNumber) = arma::mean(temp.col(0));
        matrixStddev.at(0, colNumber) = arma::stddev(temp.col(0));
        matrixMin.at(0, colNumber) = arma::min(temp.col(0));
        matrixMax.at(0, colNumber) = arma::max(temp.col(0));
        matrixMed.at(0, colNumber) = arma::median(temp.col(0));
        matrixSucc.at(0, colNumber) = (temp.col(0).n_rows/30.0) * 100;
      } else {
        matrixMean.at(0, colNumber) = arma::datum::nan;
        matrixStddev.at(0, colNumber) = arma::datum::nan;
        matrixMin.at(0, colNumber) = arma::datum::nan;
        matrixMax.at(0, colNumber) = arma::datum::nan;
        matrixMed.at(0, colNumber) = arma::datum::nan;
        matrixSucc.at(0, colNumber) = arma::datum::nan;
      }

      ++colNumber;
    }


  matrixMean.save("./SerialEvaluations_Mean_" + hop::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixStddev.save("./SerialEvaluations_Stddev_" + hop::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixMin.save("./SerialEvaluations_Min_" + hop::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixMax.save("./SerialEvaluations_Max_" + hop::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixMed.save("./SerialEvaluations_Median_" + hop::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixSucc.save("./SerialEvaluations_Success_" + hop::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  return EXIT_SUCCESS;
}
