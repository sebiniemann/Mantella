#include <mantella>
#include <memory> // Used for std::shared_ptr
#include <iomanip> // Used for std::setw

int main() {
  // Initialises the random number generator
  mant::Rng::setRandomSeed();
  
  unsigned int numberOfDimensions = 2;
  // Creates an array, containing all 24 black box optimisation benchmark problems.
  std::array<std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>, 24> optimisationProblems = {{
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::AttractiveSectorFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::BentCigarFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::BuecheRastriginFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::CompositeGriewankRosenbrockFunctionF8F2(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::DifferentPowersFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::DiscusFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::EllipsoidalFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::EllipsoidalFunctionRotated(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::GallaghersGaussian101mePeaksFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::GallaghersGaussian21hiPeaksFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::KatsuuraFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::LinearSlope(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::LunacekBiRastriginFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::RastriginFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::RastriginFunctionRotated(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::RosenbrockFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::RosenbrockFunctionRotated(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::SchaffersF7Function(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::SchaffersF7FunctionIllConditioned(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::SchwefelFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::SharpRidgeFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::SphereFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::StepEllipsoidalFunction(numberOfDimensions)),
    std::shared_ptr<mant::bbob::BlackBoxOptimisationBenchmark>(new mant::bbob::WeierstrassFunction(numberOfDimensions))
  }};
  
  // Evaluates the performance of the Hooke-Jeeves algorithm.
  mant::HillClimbing optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(1000);
  
  // Reruns the evaluation 10 times, to address random effects and stores the number of iterations in numberOfIterations.
  unsigned int numberOfReruns = 10;
  arma::Mat<arma::uword> numberOfIterations(numberOfReruns, optimisationProblems.size());
  for (std::size_t n = 0; n < optimisationProblems.size(); ++n) {
    for (unsigned int k = 0; k < numberOfReruns; ++k) {
      // Sets the acceptable objective value to a maximal deviation of 0.1 towards the optimal objective value.
      optimisationAlgorithm.setAcceptableObjectiveValue(optimisationProblems.at(n)->getOptimalObjectiveValue() + 1e-1);
      optimisationAlgorithm.optimise(*optimisationProblems.at(n));
      // Stores the number of iteration needed - each column stands for an optimisation problem and each row for a run.
      numberOfIterations(k, n) = optimisationAlgorithm.getNumberOfIterations();
    }
  }

  // Prints some statistics (min, median, max).
  std::cout << "Problem name / Number of iterations:                   | Minimal | Median | Maximal\n";
  for (std::size_t n = 0; n < optimisationProblems.size(); ++n) {
    // arma::median will have the same return type as its input, but does not ensure that the value fits, so we need to cast its input to an appropriate type.
    std::cout << std::setw(54) << std::left << optimisationProblems.at(n)->getObjectiveFunctionName() << " | " << std::setw(7) << arma::min(numberOfIterations.col(n)) << " | " << std::setw(6) << arma::median(arma::conv_to<arma::Col<double>>::from(numberOfIterations.col(n))) << " | " << arma::max(numberOfIterations.col(n)) << '\n'; 
  }
  std::cout << std::flush;
  
  return 0;
}
