#include <armadillo>
#include <mantella>

int main(int argc, char** argv) {
  //  if (argc != 2) {
  //    throw std::invalid_argument("The number of arguments (" + std::to_string(argc) + ") provided must be exactly 1.");
  //  }
  try {
    arma::arma_rng::set_seed_random();
    unsigned int numberOfDimensions = 10; //std::stoi(argv[1]);
    unsigned int lambda = 4 + std::floor(3 * std::log(numberOfDimensions));

    std::shared_ptr<mant::bbob2013::BlackBoxOptimisationBenchmark2013> optProblem(new mant::bbob2013::SphereFunction(numberOfDimensions));
    optProblem->setTranslation(arma::zeros<arma::Col<double>>(optProblem->getNumberOfDimensions()));

    //std::shared_ptr<mant::OptimisationProblem> optProblem(new mant::bbob2013::SphereFunction(numberOfDimensions));

    mant::CovarianceMatrixAdaptationEvolutionStrategy<mant::EuclideanDistance> optAlgo(optProblem, lambda);

    optAlgo.optimise();

    arma::Col<double> results(6 + numberOfDimensions);
    results.at(0) = optProblem->getNumberOfEvaluations();
    results.at(1) = optProblem->getNumberOfDistinctEvaluations();
    results.at(2) = optAlgo.getNumberOfIterations();
    results.at(3) = optAlgo.getBestObjectiveValue() - optProblem->getAcceptableObjectiveValue();
    results.at(4) = optAlgo.isFinished();
    results.at(5) = optAlgo.isTerminated();
    results(arma::span(6, 6 + numberOfDimensions - 1)) = optAlgo.getBestParameter();

    std::cout << results.at(2) << std::endl;
    std::cout << results.at(3) << std::endl;

    //    std::cout << results << std::endl;
    //
    //    std::cout << "acceptable: " << optProblem->getAcceptableObjectiveValue() << std::endl;
    //    std::cout << "best: " << optAlgo.getBestObjectiveValue() << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception& exception) {
    std::cout << exception.what();
    std::cout << "\n";
  }

}

