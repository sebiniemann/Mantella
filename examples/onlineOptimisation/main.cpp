#include <cstdlib>
#include <memory>
#include <iostream>

#include <hop>

int main(int argc, char* argv[]) {
  try {
    hop::Rng::setRandomSeed();

    hop::MaximumLikelihoodEstimation propertiesEstimation(
      std::shared_ptr<hop::CorrelationAnalysis>(new hop::FitnessDistanceCorrelationAnalysis()),
      std::shared_ptr<hop::LinearModelAnalysis>(new hop::LinearOrdinaryLeastSquares()),
      std::shared_ptr<hop::QuadraticModelAnalysis>(new hop::QuadraticOrdinaryLeastSquares()),
      std::shared_ptr<hop::LipschitzContinuityAnalysis>(new hop::DirectLipschitzContinuityAnalysis()),
      std::shared_ptr<hop::AdditiveSeparabilityAnalysis>(new hop::DirectAdditiveSeparabilityAnalysis()));

    std::shared_ptr<hop::OptimisationProblem> optimisationProblem(new hop::bbob2013::SphereFunction(4));

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
      std::cout << "correlationCoefficient: " << propertiesEstimation.getCorrelationCoefficient(n) << std::endl;
      std::cout << "lipschitzConstant: " << propertiesEstimation.getLipschitzConstant(n) << std::endl;
      std::cout << "isLinear: " << propertiesEstimation.isLinear(n) << std::endl;
      std::cout << "linearModelEstimator: " << propertiesEstimation.getLinearModelEstimator(n) << std::endl;
      std::cout << "isQuaratic:" << propertiesEstimation.isQuadratic(n) << std::endl;
      std::cout << "quadraticModelEstimator" << propertiesEstimation.getQuadraticModelEstimator(n) << std::endl;
    }

    return EXIT_SUCCESS;
  } catch(const std::exception& exception) {
    std::cout << exception.what();
  }
}
