// C++ Standard Library
#include <cstdlib>
#include <memory>
#include <iostream>
#include <limits>

// Mantella
#include <mantella>

int main(int argc, char** argv) {
  unsigned int numberOfDimensions = 10;
  
  std::shared_ptr<mant::bbob2013::BlackBoxOptimisationBenchmark2013> optProblem(new mant::bbob2013::SphereFunction(numberOfDimensions));
  
  unsigned int popSize = 3;
  arma::Mat<double> boundaries(numberOfDimensions,2);
  arma::Col<arma::uword> initialPointIndex = arma::Col<arma::uword>(numberOfDimensions,2);
  
  mant::MultilevelCoordinateSearch<mant::EuclideanDistance> optAlgo(optProblem,popSize,boundaries,initialPointIndex);
}
