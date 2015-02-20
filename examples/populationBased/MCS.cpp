#include <armadillo>
#include <mantella>

int main(int argc, char** argv) {
  try {
    unsigned int numberOfDimensions = 2;

    std::shared_ptr<mant::bbob2009::BlackBoxOptimisationBenchmark2009> optProblem(new mant::bbob2009::SphereFunction(numberOfDimensions));
    optProblem->setTranslation(arma::zeros<arma::Col<double>>(optProblem->getNumberOfDimensions()));
    optProblem->setObjectiveValueTranslation(0);
    //optProblem->setAcceptableObjectiveValue(0 + 1.0e-8);

    unsigned int popSize = 3;
    arma::Col<arma::uword> initialPointIndex(numberOfDimensions, arma::fill::ones);
    std::cout << "creating MCS object" << std::endl;
    mant::MultilevelCoordinateSearch<mant::EuclideanDistance> optAlgo(optProblem, popSize, initialPointIndex);
    
    optAlgo.setMaximalNumberOfIterations(20);
    std::cout << "starting optimise" << std::endl;
    optAlgo.optimise();
    std::cout << "optimise done" << std::endl;
    arma::Col<double> results(6 + numberOfDimensions);
    results.at(0) = optProblem->getNumberOfEvaluations();
    results.at(1) = optProblem->getNumberOfDistinctEvaluations();
    results.at(2) = optAlgo.getNumberOfIterations();
    results.at(3) = optAlgo.getBestObjectiveValue() - optProblem->getAcceptableObjectiveValue();
    results.at(4) = optAlgo.isFinished();
    results.at(5) = optAlgo.isTerminated();
    results(arma::span(6, 6 + numberOfDimensions - 1)) = optAlgo.getBestParameter();

    std::cout << "acceptable = " << optProblem->getAcceptableObjectiveValue() << std::endl;
    std::cout << "xbest = " << optAlgo.getBestParameter() << std::endl;
    std::cout << "fbest = " << optAlgo.getBestObjectiveValue() << std::endl;
    std::cout << "ncall = " << results.at(2) << std::endl;
    

    return EXIT_SUCCESS;
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}

