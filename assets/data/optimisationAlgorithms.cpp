#include <mantella>

int main() {
  ::mant::isRecordingSampling = true;
  
  mant::OptimisationProblem optimisationProblem(2);
  optimisationProblem.setLowerBounds({-3, -3});
  optimisationProblem.setUpperBounds({3, 3});
  optimisationProblem.setObjectiveFunction(
    [] (
        const arma::Col<double> parameter) {
      double x = parameter(0);
      double y = parameter(1);
        
      return 3.0 * std::pow(1.0 - x, 2.0) * std::exp(-std::pow(x, 2.0) - std::pow(y + 1.0, 2.0)) - 10.0 * (x/5.0 - std::pow(x, 3.0) - std::pow(y, 5.0)) * std::exp(-std::pow(x, 2.0) - std::pow(y, 2.0)) - 1.0/3.0 * std::exp(-std::pow(x + 1.0, 2.0) - std::pow(y, 2.0));
  });
  
  std::ofstream outputStream;
  
  /**
   * Random search
   */
  
  mant::RandomSearch randomSearch;
  randomSearch.setMaximalNumberOfIterations(10000);
  randomSearch.optimise(optimisationProblem);
  
  outputStream.open("./randomSearch.json");
  outputStream << "[";
  auto samples = randomSearch.getRecordedSampling();
  for (arma::uword n = 0; n < samples.size(); ++n) {
    const auto& sample = samples.at(n);
    outputStream << "[" << sample.first(0) << ", " << sample.first(1) << "]";
    if (n < samples.size() - 1) {
      outputStream << ", ";
    }
  }
  outputStream << "]";
  outputStream.close();
  
  /**
   * Hill climbing
   */
  
  mant::HillClimbing hillClimbing;
  hillClimbing.setMaximalNumberOfIterations(10000);
  hillClimbing.optimise(optimisationProblem);
  
  outputStream.open("./hillClimbing.json");
  outputStream << "[";
  samples = hillClimbing.getRecordedSampling();
  for (arma::uword n = 0; n < samples.size(); ++n) {
    const auto& sample = samples.at(n);
    outputStream << "[" << sample.first(0) << ", " << sample.first(1) << "]";
    if (n < samples.size() - 1) {
      outputStream << ", ";
    }
  }
  outputStream << "]";
  outputStream.close();
  
  /**
   * Simulated annealing
   */
   
  mant::SimulatedAnnealing simulatedAnnealing;
  simulatedAnnealing.setMaximalNumberOfIterations(10000);
  simulatedAnnealing.optimise(optimisationProblem);
  
  outputStream.open("./simulatedAnnealing.json");
  outputStream << "[";
  samples = simulatedAnnealing.getRecordedSampling();
  for (arma::uword n = 0; n < samples.size(); ++n) {
    const auto& sample = samples.at(n);
    outputStream << "[" << sample.first(0) << ", " << sample.first(1) << "]";
    if (n < samples.size() - 1) {
      outputStream << ", ";
    }
  }
  outputStream << "]";
  outputStream.close();
  
  /**
   * Hooke-Jeeves algorithm
   */
  
  // mant::HookeJeevesAlgorithm hookeJeevesAlgorithm;
  // hookeJeevesAlgorithm.setMaximalNumberOfIterations(10000);
  // hookeJeevesAlgorithm.optimise(optimisationProblem);
  
  // outputStream.open("./hookeJeevesAlgorithm.json");
  // outputStream << "[";
  // samples = hookeJeevesAlgorithm.getRecordedSampling();
  // for (arma::uword n = 0; n < samples.size(); ++n) {
    // const auto& sample = samples.at(n);
    // outputStream << "[" << sample.first(0) << ", " << sample.first(1) << "]";
    // if (n < samples.size() - 1) {
      // outputStream << ", ";
    // }
  // }
  // outputStream << "]";
  // outputStream.close();
  
  /**
   * Particle swarm optimisation
   */
  
  mant::ParticleSwarmOptimisation particleSwarmOptimisation;
  particleSwarmOptimisation.setMaximalNumberOfIterations(10000);
  particleSwarmOptimisation.optimise(optimisationProblem, 10);
  
  outputStream.open("./particleSwarmOptimisation.json");
  outputStream << "[";
  samples = particleSwarmOptimisation.getRecordedSampling();
  for (arma::uword n = 0; n < samples.size(); ++n) {
    const auto& sample = samples.at(n);
    outputStream << "[" << sample.first(0) << ", " << sample.first(1) << "]";
    if (n < samples.size() - 1) {
      outputStream << ", ";
    }
  }
  outputStream << "]";
  outputStream.close();
  
  return 0;
}