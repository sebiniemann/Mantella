#include <cstdlib>
#include <memory>
#include <iostream>
#include <limits>

#include <hop>

int main(int argc, char* argv[]) {
  try {
    hop::Rng::setRandomSeed();

    std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::bbob2013::SphereFunction(2));
    optimisationProblem->setAcceptableObjectiveValue(-std::numeric_limits<double>::infinity());

    hop::MultiResolutionGridSearch multiResolutionGridSearch(optimisationProblem);
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
