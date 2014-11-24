#include <cstdlib>
#include <memory>
#include <iostream>

#include <hop>

int main(int argc, char* argv[]) {
  try {
    hop::Rng::setRandomSeed();

    hop::MaximumLikelihoodEstimation propertiesEstimation(
          std::shared_ptr<hop::LinearModelAnalysis>(new hop::LinearOrdinaryLeastSquares()),
          std::shared_ptr<hop::QuadraticModelAnalysis>(new hop::QuadraticOrdinaryLeastSquares()),
          std::shared_ptr<hop::LipschitzContinuityAnalysis>(new hop::DirectLipschitzContinuityAnalysis()));

    std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::bbob2013::LinearSlope(2));

    hop::GridSearch gridSearch(optimisationProblem);
    gridSearch.optimise();

    std::cout << "isFinished: " << gridSearch.isFinished() << std::endl;
    std::cout << "isTerminated: " << gridSearch.isTerminated() << std::endl;
    std::cout << "numberOfIterations: " << gridSearch.getNumberOfIterations() << std::endl;
    std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
    std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
    std::cout << "bestSoftConstraintsValue: " << gridSearch.getBestSoftConstraintsValue() << std::endl;
    std::cout << "bestObjectiveValueDelta: " << gridSearch.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
    std::cout << "bestParameter: " << gridSearch.getBestParameter() << std::endl;


    hop::HookeJeevesAlgorithm optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();

    std::cout << "isFinished: " << optimisationAlgorithm.isFinished() << std::endl;
    std::cout << "isTerminated: " << optimisationAlgorithm.isTerminated() << std::endl;
    std::cout << "numberOfIterations: " << optimisationAlgorithm.getNumberOfIterations() << std::endl;
    std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
    std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
    std::cout << "bestSoftConstraintsValue: " << optimisationAlgorithm.getBestSoftConstraintsValue() << std::endl;
    std::cout << "bestObjectiveValueDelta: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
    std::cout << "bestParameter: " << optimisationAlgorithm.getBestParameter() << std::endl;

    propertiesEstimation.estimate(optimisationProblem);
    std::cout << "getNumberOfPropertySets: " << propertiesEstimation.getNumberOfPropertySets() << std::endl;

    for(std::size_t n = 0; n < propertiesEstimation.getNumberOfPropertySets(); ++n) {
      std::cout << "isLinear: " << propertiesEstimation.isLinear(n) << std::endl;
      std::cout << "linearModelEstimator: " << propertiesEstimation.getLinearModelEstimator(n) << std::endl;
      std::cout << "isQuaratic:" << propertiesEstimation.isQuadratic(n) << std::endl;
      std::cout << "quadraticModelEstimator" << propertiesEstimation.getQuadraticModelEstimator(n) << std::endl;
      std::cout << "isLipschitzContinuous: " << propertiesEstimation.isLipschitzContinuous(n) << std::endl;
      std::cout << "lipschitzConstant: " << propertiesEstimation.getLipschitzConstant(n) << std::endl;
    }

    std::shared_ptr<hop::OptimisationProblem> optimisationProblem2(new hop::bbob2013::SphereFunction(2));

    hop::HookeJeevesAlgorithm optimisationAlgorithm2(optimisationProblem2);
    optimisationAlgorithm2.optimise();

    std::cout << "isFinished: " << optimisationAlgorithm2.isFinished() << std::endl;
    std::cout << "isTerminated: " << optimisationAlgorithm2.isTerminated() << std::endl;
    std::cout << "numberOfIterations: " << optimisationAlgorithm2.getNumberOfIterations() << std::endl;
    std::cout << "numberOfEvaluations: " << optimisationProblem2->getNumberOfEvaluations() << std::endl;
    std::cout << "numberOfDistinctEvaluations: " << optimisationProblem2->getNumberOfDistinctEvaluations() << std::endl;
    std::cout << "bestSoftConstraintsValue: " << optimisationAlgorithm2.getBestSoftConstraintsValue() << std::endl;
    std::cout << "bestObjectiveValueDelta: " << optimisationAlgorithm2.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
    std::cout << "bestParameter: " << optimisationAlgorithm2.getBestParameter() << std::endl;

    propertiesEstimation.estimate(optimisationProblem2);
    std::cout << "getNumberOfPropertySets: " << propertiesEstimation.getNumberOfPropertySets() << std::endl;

    for(std::size_t n = 0; n < propertiesEstimation.getNumberOfPropertySets(); ++n) {
      std::cout << "isLinear: " << propertiesEstimation.isLinear(n) << std::endl;
      std::cout << "linearModelEstimator: " << propertiesEstimation.getLinearModelEstimator(n) << std::endl;
      std::cout << "isQuaratic:" << propertiesEstimation.isQuadratic(n) << std::endl;
      std::cout << "quadraticModelEstimator" << propertiesEstimation.getQuadraticModelEstimator(n) << std::endl;
      std::cout << "isLipschitzContinuous: " << propertiesEstimation.isLipschitzContinuous(n) << std::endl;
      std::cout << "lipschitzConstant: " << propertiesEstimation.getLipschitzConstant(n) << std::endl;
    }

    return EXIT_SUCCESS;
  } catch(const std::exception& exception) {
    std::cout << exception.what();
  }
}
