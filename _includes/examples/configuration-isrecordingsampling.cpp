#include <mantella>
#include <iomanip> // Used for std::setw

int main() {
  ::mant::isRecordingSampling = true;
  
  unsigned int numberOfDimensions = 2;
  mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

  mant::RandomSearch optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(10); // Just limits the example output.
  optimisationAlgorithm.optimise(optimisationProblem);

  std::size_t n = 0;
  std::cout << "Sample Nr. | Objective value | Parameter\n";
  for (const auto& sample : optimisationAlgorithm.getRecordedSampling()) {
    std::cout << std::setw(10) << ++n << " | " << std::setw(15) << sample.second << " | " << sample.first.t();
  }
  std::cout << std::flush;

  return 0;
}
