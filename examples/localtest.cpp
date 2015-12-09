#include <cstdlib>
#include <cmath>
#include <string>
#include <memory>
#include <iostream>
#include <armadillo>
#include <cassert>
#include <mantella>

using namespace std;

int main(int argc, char** argv) {
  std::shared_ptr<mant::bbob::SphereFunction> blub = std::make_shared<mant::bbob::SphereFunction>(10);
  std::cout << "optproblem created" << std::endl;
  std::cout << "constructor:" << std::endl;
  mant::CovarianceMatrixAdaptationEvolutionStrategy cmaes;
  std::cout << "specifying settings:" << std::endl;
  cmaes.setMaximalNumberOfIterations(20000);
  cmaes.setAcceptableObjectiveValue(blub->getBestObjectiveValue());
  cmaes.setStepSize(2);
  std::cout << "optimizing:" << std::endl;
  cmaes.optimise(blub,8 * arma::randu(blub->numberOfDimensions_) - 4);
  std::cout << "acceptable: " << blub->getBestObjectiveValue() << std::endl;
  std::cout << "finished: " << cmaes.isFinished() << std::endl;
  std::cout << "iters: " << cmaes.getNumberOfIterations() << std::endl;
  std::cout << "best obj: " << cmaes.getBestObjectiveValue() << std::endl;
  std::cout << "best para: " << cmaes.getBestParameter() << std::endl;
  std::cout << "end sigma: " << cmaes.getStepSize() << std::endl;
  
  mant::HillClimbing hill;
  hill.setMaximalNumberOfIterations(1000);
  hill.setAcceptableObjectiveValue(blub->getBestObjectiveValue());
  //hill.optimise(blub);
  
  std::cout << "finished: " << hill.isFinished() << std::endl;
  std::cout << "iters: " << hill.getNumberOfIterations() << std::endl;
  std::cout << "best obj: " << hill.getBestObjectiveValue() << std::endl;
  std::cout << "best para: " << hill.getBestParameter() << std::endl;
}