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
  std::array<unsigned int, 9> population({10, 15, 20, 25, 30, 35 ,40, 45, 50});

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
  arma::Mat<double> matrixMean(1, population.size());
  arma::Mat<double> matrixStddev(1, population.size());
  arma::Mat<double> matrixMin(1, population.size());
  arma::Mat<double> matrixMax(1, population.size());
  arma::Mat<double> matrixMed(1, population.size());
  arma::Mat<double> matrixSucc(1, population.size());


  std::size_t colNumber = 0;
    for(auto populationSize : population) {

      boost::filesystem::path filepath = boost::filesystem::path("/homes/shuka/Desktop/OnlineOptimisation/bin/Final/Evaluation/serialNumberOfIterations_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions) + "_pop" + std::to_string(populationSize) + ".mat");

      if(!boost::filesystem::exists(filepath)) {
            // TODO Add exception
      }

      arma::Mat<double> temp;
      temp.load("/homes/shuka/Desktop/OnlineOptimisation/bin/Final/Evaluation/serialNumberOfIterations_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions) + "_pop" + std::to_string(populationSize) + ".mat");
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


  matrixMean.save("./SerialEvaluations_Mean_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixStddev.save("./SerialEvaluations_Stddev_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixMin.save("./SerialEvaluations_Min_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixMax.save("./SerialEvaluations_Max_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixMed.save("./SerialEvaluations_Median_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  matrixSucc.save("./SerialEvaluations_Success_" + mant::to_string(optimisationProblem)  + "_dim" + std::to_string(numberOfDimensions), arma::raw_ascii);
  return EXIT_SUCCESS;
}
