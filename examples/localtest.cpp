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
//  mant::bbob::SphereFunction blub(10);
//  std::cout << "optproblem created" << std::endl;
//  std::cout << "constructor:" << std::endl;
//  mant::CovarianceMatrixAdaptationEvolutionStrategy cmaes;
//  std::cout << "specifying settings:" << std::endl;
//  cmaes.setMaximalNumberOfIterations(20000);
//  const double acceptableObjectiveValue = blub.getBestObjectiveValue() + std::pow(10.0, std::floor(std::log10(std::abs(blub.getBestObjectiveValue())))) * 1e-3;
//  cmaes.setAcceptableObjectiveValue(acceptableObjectiveValue);
//  cmaes.setStepSize(2);
//  std::cout << "optimizing:" << std::endl;
//  cmaes.optimise(blub,8 * arma::randu(blub.numberOfDimensions_) - 4);
//  std::cout << "acceptable: " << acceptableObjectiveValue << std::endl;
//  std::cout << "finished: " << cmaes.isFinished() << std::endl;
//  std::cout << "iters: " << cmaes.getNumberOfIterations() << std::endl;
//  std::cout << "best obj: " << cmaes.getBestObjectiveValue() << std::endl;
//  std::cout << "best para: " << cmaes.getBestParameter() << std::endl;
//  std::cout << "end sigma: " << cmaes.getStepSize() << std::endl;
  
  //benchmarking our cmaes
  std::vector<std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>> optimisationProblems = getBenchmarkOptimisationProblems(5);
  arma::Col<double> medians(optimisationProblems.size());
  for(unsigned int i = 0; i < optimisationProblems.size(); i++) {
    std::cout << "function " << i+1 << std::endl;
    
    mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem = *optimisationProblems.at(i);
    
    //std::cout << optimisationProblem.getBestObjectiveValue() << std::endl;
    double acceptableObjectiveValue = optimisationProblem.getBestObjectiveValue() + std::pow(10.0, std::floor(std::log10(std::abs(optimisationProblem.getBestObjectiveValue())))) * 1e-3;
    
    arma::Col<double> curMedian(20);
 
    for(int trial = 0; trial < 20; trial++) {
      mant::CovarianceMatrixAdaptationEvolutionStrategy algo;
      algo.setMaximalNumberOfIterations(20000);
      algo.setAcceptableObjectiveValue(acceptableObjectiveValue);
      algo.setStepSize(2);
      algo.optimise(optimisationProblem,8 * arma::randu(optimisationProblem.numberOfDimensions_) - 4);
      curMedian(trial) = algo.getNumberOfIterations();          

      // std::cout << algo.getNumberOfIterations() << std::endl;
       //std::cout << "distance to acceptable objValue " << acceptableObjectiveValue - algo.getBestObjectiveValue() << std::endl;
    }

    //std::cout << arma::median(curMedian) << std::endl;
    medians(i) = arma::median(curMedian);
  }
  
  std::cout << medians.t() << std::endl;
 
  return 0;
}