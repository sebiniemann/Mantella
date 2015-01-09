// C++ Standard Library
#include <cstdlib>
#include <memory>
#include <iostream>
#include <limits>

// Mantella
#include <mantella>

int main(int argc, char* argv[]) {
  try {
    mant::Rng::setRandomSeed();

    std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new mant::bbob2013::SphereFunction(2));
    optimisationProblem->setAcceptableObjectiveValue(-std::numeric_limits<double>::infinity());

    mant::MultiResolutionGridSearch multiResolutionGridSearch(optimisationProblem);
    multiResolutionGridSearch.setMaximalNumberOfIterations(100000);
    multiResolutionGridSearch.optimise();

    std::cout << "isFinished: " << multiResolutionGridSearch.isFinished() << std::endl;
    std::cout << "isTerminated: " << multiResolutionGridSearch.isTerminated() << std::endl;
    std::cout << "numberOfIterations: " << multiResolutionGridSearch.getNumberOfIterations() << std::endl;
    std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
    std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
    std::cout << "bestSoftConstraintsValue: " << multiResolutionGridSearch.getBestSoftConstraintsValue() << std::endl;
    std::cout << "bestObjectiveValueDelta: " << multiResolutionGridSearch.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
    std::cout << "bestParameter: " << multiResolutionGridSearch.getBestParameter() << std::endl;

    return EXIT_SUCCESS;
  } catch(const std::exception& exception) {
    std::cout << exception.what();
  }
}
