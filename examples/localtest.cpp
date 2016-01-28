#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>
#include <memory>
#include <iostream>
#include <armadillo>
#include <cassert>
#include <mantella>
#include <memory>

using namespace std;

std::vector<std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>> getBenchmarkOptimisationProblems(arma::uword numberOfDimensions) {
    std::vector<std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>> optimisationProblems;
    
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::SphereFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::EllipsoidalFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::RastriginFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::BuecheRastriginFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::LinearSlope(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::AttractiveSectorFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::StepEllipsoidalFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::RosenbrockFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::RosenbrockFunctionRotated(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::EllipsoidalFunctionRotated(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::DiscusFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::BentCigarFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::SharpRidgeFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::DifferentPowersFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::RastriginFunctionRotated(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::WeierstrassFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::SchaffersF7Function(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::SchaffersF7FunctionIllConditioned(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::CompositeGriewankRosenbrockFunctionF8F2(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::SchwefelFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::GallaghersGaussian101mePeaksFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::GallaghersGaussian21hiPeaksFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::KatsuuraFunction(numberOfDimensions)));
    optimisationProblems.push_back(std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new  mant::bbob::LunacekBiRastriginFunction(numberOfDimensions)));

    return optimisationProblems;
}

int main() {  
  mant::isVerbose = false;
  std::cout.precision(16);
  
  //benchmarking our cmaes
  arma::uword dimensions = 10;
  std::vector<std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>> optimisationProblems = getBenchmarkOptimisationProblems(dimensions);
  arma::Col<double> medians(optimisationProblems.size());
  arma::Col<arma::uword> sucessfulRuns(optimisationProblems.size());
  arma::Mat<double> startingPoint = arma::ones(dimensions,dimensions);
  startingPoint = startingPoint.each_col() % (8 * arma::randu(optimisationProblems.at(0)->numberOfDimensions_) - 4);
  std::cout << "startingPoint" << startingPoint << std::endl;
  
  for(unsigned int i = 0; i < 24; i++) {
    std::cout << "function " << i+1 << std::endl;
    
    mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem = *optimisationProblems.at(i);
    
    //std::cout << optimisationProblem.getOptimalObjectiveValue() << std::endl;
    double acceptableObjectiveValue = optimisationProblem.getOptimalObjectiveValue() + std::pow(10.0, std::floor(std::log10(std::abs(optimisationProblem.getOptimalObjectiveValue())))) * 1e-3;
    
    int trials = 20;
    arma::Col<double> curMedian(trials);
    
 
    for(int trial = 0; trial < trials; trial++) {
      mant::CovarianceMatrixAdaptationEvolutionStrategy algo;
      algo.setMaximalDuration(std::chrono::minutes(20));
      algo.setMaximalNumberOfIterations(20000);
      algo.setAcceptableObjectiveValue(acceptableObjectiveValue);
      algo.setInitialStepSize(2);
      algo.setActiveCMA(false);
//      std::cout << "nothing" << std::endl;
      algo.optimise(optimisationProblem);
//      std::cout << "Mat" << std::endl;
//      algo.optimise(optimisationProblem,startingPoint);
//      std::cout << "Col" << std::endl;
//      arma::Col<double> testcol = 8 * arma::randu(optimisationProblems.at(0)->numberOfDimensions_) - 4;
//      algo.optimise(optimisationProblem,testcol);
//      std::cout << "double" << std::endl;
//      algo.optimise(optimisationProblem,10.0);
//      std::cout << "Col&double" << std::endl;
//      algo.optimise(optimisationProblem,testcol,10.0);
      curMedian(trial) = algo.getNumberOfIterations();
      sucessfulRuns(i) += algo.isFinished();
      
       //std::cout << algo.getNumberOfIterations() << std::endl;
      //std::cout << "isFinished" << algo.isFinished() << std::endl;
      //std::cout << "distance to acceptable objValue " << acceptableObjectiveValue - algo.getBestObjectiveValue() << std::endl;
    }

    medians(i) = arma::median(curMedian);
    std::cout << "median iterations for function: " << arma::median(curMedian) << std::endl;
  }
  
  std::cout << medians.t() << std::endl;
 
  return 0;
}