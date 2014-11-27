#include <armadillo>
#include <hop>

int main(int argc, char** argv) {
  if (argc != 3) {
    throw std::invalid_argument("The number of arguments (" + std::to_string(argc) + ") provided must be exactly 2.");
  }
  try {
    arma::arma_rng::set_seed_random();
    unsigned int numberOfDimensions = std::stoi(argv[1]);
    double stepSize = std::stoi(argv[2]);

    std::shared_ptr<hop::OptimisationProblem> optProblem(new hop::bbob2013::RosenbrockFunction(numberOfDimensions));
    //TODO: CMAES determines population according to dimension, what to do here?
    hop::CovarianceMatrixAdaptationEvolutionStrategy optAlgo(optProblem, 0, stepSize);

    optAlgo.optimise();

    arma::Col<double> results(7);
    results.at(0) = optProblem->getNumberOfEvaluations();
    results.at(1) = optProblem->getNumberOfDistinctEvaluations();
    results.at(2) = optAlgo.getNumberOfIterations();
    results.at(3) = optAlgo.getBestObjectiveValue() - optProblem->getAcceptableObjectiveValue();
    results.at(4) = optAlgo.isFinished();
    results.at(5) = optAlgo.isTerminated();
    results(arma::span(6, 6 + numberOfDimensions - 1)) = optAlgo.getBestParameter().t();

    for (int i = 0; i < 7; i++) {
      std::cout << results(i);
    }

    return EXIT_SUCCESS;
  } catch (const std::exception& exception) {
    std::cout << exception.what();
    std::cout << "\n";
  }

}

