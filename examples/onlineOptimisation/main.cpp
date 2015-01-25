#include <cstdlib>
#include <memory>
#include <iostream>

#include <mantella>

int main(int argc, char* argv[]) {
  try {
    mant::Rng::setRandomSeed();

    mant::MaximumLikelihoodEstimation<double, mant::EuclideanDistance> propertiesEstimation(
      std::shared_ptr<mant::CorrelationAnalysis<double, mant::EuclideanDistance>>(new mant::FitnessDistanceCorrelationAnalysis()),
      std::shared_ptr<mant::LipschitzContinuityAnalysis<double, mant::EuclideanDistance>>(new mant::DirectLipschitzContinuityAnalysis<double, mant::EuclideanDistance>()),
      std::shared_ptr<mant::LinearModelAnalysis<double, mant::EuclideanDistance>>(new mant::LinearOrdinaryLeastSquares()),
      std::shared_ptr<mant::QuadraticModelAnalysis<double, mant::EuclideanDistance>>(new mant::QuadraticOrdinaryLeastSquares()),
      std::shared_ptr<mant::AdditiveSeparabilityAnalysis<double, mant::EuclideanDistance>>(new mant::DirectAdditiveSeparabilityAnalysis()));

    std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new mant::bbob2015::SphereFunction(4));

    mant::HookeJeevesAlgorithm<mant::EuclideanDistance> optimisationAlgorithm(optimisationProblem);
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
