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

int main(int argc, char** argv) {
  mant::bbob::SphereFunction blub(10);
  std::cout << "optproblem created" << std::endl;
  std::cout << "constructor:" << std::endl;
  mant::CovarianceMatrixAdaptationEvolutionStrategy cmaes;
  std::cout << "specifying settings:" << std::endl;
  cmaes.setMaximalNumberOfIterations(20000);
  const double acceptableObjectiveValue = blub.getBestObjectiveValue() + std::pow(10.0, std::floor(std::log10(std::abs(blub.getBestObjectiveValue())))) * 1e-3;
  cmaes.setAcceptableObjectiveValue(acceptableObjectiveValue);
  cmaes.setStepSize(2);
  std::cout << "optimizing:" << std::endl;
  cmaes.optimise(blub,8 * arma::randu(blub.numberOfDimensions_) - 4);
  std::cout << "acceptable: " << acceptableObjectiveValue << std::endl;
  std::cout << "finished: " << cmaes.isFinished() << std::endl;
  std::cout << "iters: " << cmaes.getNumberOfIterations() << std::endl;
  std::cout << "best obj: " << cmaes.getBestObjectiveValue() << std::endl;
  std::cout << "best para: " << cmaes.getBestParameter() << std::endl;
  std::cout << "end sigma: " << cmaes.getStepSize() << std::endl;
  
  mant::HillClimbing hill;
  hill.setMaximalNumberOfIterations(1000);
  hill.setAcceptableObjectiveValue(blub.getBestObjectiveValue());
  //hill.optimise(blub);
  
  std::cout << "finished: " << hill.isFinished() << std::endl;
  std::cout << "iters: " << hill.getNumberOfIterations() << std::endl;
  std::cout << "best obj: " << hill.getBestObjectiveValue() << std::endl;
  std::cout << "best para: " << hill.getBestParameter() << std::endl;
}