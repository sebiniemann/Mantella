#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>
#include <memory>
#include <iostream>
#include <armadillo>
#include <cassert>
#include <mantella>

using namespace std;

std::vector<mant::bbob::BlackBoxOptimisationBenchmark> getBenchmarkProblems(arma::uword dim) {
    std::vector<mant::bbob::BlackBoxOptimisationBenchmark> probs;
    probs.push_back(mant::bbob::SphereFunction(dim));
    probs.push_back(mant::bbob::EllipsoidalFunction(dim));
    probs.push_back(mant::bbob::RastriginFunction(dim));
    probs.push_back(mant::bbob::BuecheRastriginFunction(dim));
    probs.push_back(mant::bbob::LinearSlope(dim));
    probs.push_back(mant::bbob::AttractiveSectorFunction(dim));
    probs.push_back(mant::bbob::StepEllipsoidalFunction(dim));
    probs.push_back(mant::bbob::RosenbrockFunction(dim));
    probs.push_back(mant::bbob::RosenbrockFunctionRotated(dim));
    probs.push_back(mant::bbob::EllipsoidalFunctionRotated(dim));
    probs.push_back(mant::bbob::DiscusFunction(dim));
    probs.push_back(mant::bbob::BentCigarFunction(dim));
    probs.push_back(mant::bbob::SharpRidgeFunction(dim));
    probs.push_back(mant::bbob::DifferentPowersFunction(dim));
    probs.push_back(mant::bbob::RastriginFunctionRotated(dim));
    probs.push_back(mant::bbob::WeierstrassFunction(dim));
    probs.push_back(mant::bbob::SchaffersF7Function(dim));
    probs.push_back(mant::bbob::SchaffersF7FunctionIllConditioned(dim));
    probs.push_back(mant::bbob::CompositeGriewankRosenbrockFunctionF8F2(dim));
    probs.push_back(mant::bbob::SchwefelFunction(dim));
    probs.push_back(mant::bbob::GallaghersGaussian101mePeaksFunction(dim));
    probs.push_back(mant::bbob::GallaghersGaussian21hiPeaksFunction(dim));
    probs.push_back(mant::bbob::KatsuuraFunction(dim));
    probs.push_back(mant::bbob::LunacekBiRastriginFunction(dim));

    return probs;
}

mant::bbob::BlackBoxOptimisationBenchmark getBenchmarkProblem(arma::uword dim, int functionNumber) {
    switch(functionNumber) {
        case 0:
            return mant::bbob::SphereFunction(dim);
            break;
        case 1:
            return mant::bbob::EllipsoidalFunction(dim);
            break;
        case 2:
            return mant::bbob::RastriginFunction(dim);
            break;
        case 3:
            return mant::bbob::BuecheRastriginFunction(dim);
            break;
        case 4:
            return mant::bbob::LinearSlope(dim);
            break;
        case 5:
            return mant::bbob::AttractiveSectorFunction(dim);
            break;
        case 6:
            return mant::bbob::StepEllipsoidalFunction(dim);
            break;
        case 7:
            return mant::bbob::RosenbrockFunction(dim);
            break;
        case 8:
            return mant::bbob::RosenbrockFunctionRotated(dim);
            break;
        case 9:
            return mant::bbob::EllipsoidalFunctionRotated(dim);
            break;
        case 10:
            return mant::bbob::DiscusFunction(dim);
            break;
        case 11:
            return mant::bbob::BentCigarFunction(dim);
            break;
        case 12:
            return mant::bbob::SharpRidgeFunction(dim);
            break;
        case 13:
            return mant::bbob::DifferentPowersFunction(dim);
            break;
        case 14:
            return mant::bbob::RastriginFunctionRotated(dim);
            break;
        case 15:
            return mant::bbob::WeierstrassFunction(dim);
            break;
        case 16:
            return mant::bbob::SchaffersF7Function(dim);
            break;
        case 17:
            return mant::bbob::SchaffersF7FunctionIllConditioned(dim);
            break;
        case 18:
            return mant::bbob::CompositeGriewankRosenbrockFunctionF8F2(dim);
            break;
        case 19:
            return mant::bbob::SchwefelFunction(dim);
            break;
        case 20:
            return mant::bbob::GallaghersGaussian101mePeaksFunction(dim);
            break;
        case 21:
            return mant::bbob::GallaghersGaussian21hiPeaksFunction(dim);
            break;
        case 22:
            return mant::bbob::KatsuuraFunction(dim);
            break;
        case 23:
            return mant::bbob::LunacekBiRastriginFunction(dim);
            break;            
    }
    return mant::bbob::SphereFunction(dim);
}

int main(int argc, char** argv) {
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
  //std::vector<mant::bbob::BlackBoxOptimisationBenchmark> problems = getBenchmarkProblems(5);
//  arma::Col<double> medians(24);
//  for(int i = 0; i < 24; i++) {
//      std::cout << "function " << i << std::endl;
      mant::bbob::AttractiveSectorFunction curProb = mant::bbob::AttractiveSectorFunction(40);// = problems[i];
      std::cout << curProb.getBestObjectiveValue() << std::endl;
      double objValue = curProb.getBestObjectiveValue() + std::pow(10.0, std::floor(std::log10(std::abs(curProb.getBestObjectiveValue())))) * 1e-3;
      arma::Col<double> curMedian(20);
      for(int trial = 0; trial < 20; trial++) {
          mant::CovarianceMatrixAdaptationEvolutionStrategy algo;
          algo.setMaximalNumberOfIterations(20000);
          algo.setAcceptableObjectiveValue(objValue);
          algo.setStepSize(2);
          algo.optimise(curProb,8 * arma::randu(curProb.numberOfDimensions_) - 4);
          curMedian(trial) = algo.getNumberOfIterations();          
          
          std::cout << algo.getNumberOfIterations() << std::endl;
          std::cout << algo.getBestObjectiveValue() << std::endl;
      }
      
      std::cout << arma::median(curMedian) << std::endl;
//      medians(i) = arma::median(curMedian);
//  }
  
//  std::cout << medians << std::endl;
}