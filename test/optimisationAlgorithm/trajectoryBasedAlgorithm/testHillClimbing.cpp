// Catch
#include <catch.hpp>

// C++ Standard Library
#include <memory>
#include <iostream>

// Boost
#include <boost/filesystem.hpp>

// Armadillo
#include <armadillo>

// HOP
#include <hop>

extern boost::filesystem::path testDirectory;
static std::string dataPath_ = "/data/optimisationAlgorithm/trajectoryBasedAlgrorithm/hillClimbing/";

class TestHillClimbing : public hop::HillClimbing {
  public:
    TestHillClimbing(
        const std::shared_ptr<hop::OptimisationProblem> optimisationProblem)
      : HillClimbing(optimisationProblem),
        velocityIndex_(0){
      velocities_.load(testDirectory.string() + dataPath_ + "velocities.mat");
    }

    arma::Col<double> getVelocity() {
      return velocities_.col(velocityIndex_++);
    }

  protected:
    unsigned int velocityIndex_;
    arma::mat velocities_;
};

class TestHillClimbingProblem : public hop::OptimisationProblem {
  public:
    TestHillClimbingProblem(
        const unsigned int numberOfDimensions)
      : OptimisationProblem(numberOfDimensions),
        ValueIndex_(0) {
      objectiveValues_.load(testDirectory.string() + dataPath_ + "objectiveValues.mat");
      softConstraintsValues_.load(testDirectory.string() + dataPath_ + "softConstraintsValues.mat");
    }


    std::vector<arma::Col<double>> getParameterHistory() const noexcept {
      return parameterHistory_;
    }

  protected:
    unsigned int ValueIndex_;
    arma::Col<double> objectiveValues_;
    arma::Col<double> softConstraintsValues_;

    static std::vector<arma::Col<double>> parameterHistory_;

    double getObjectiveValueImplementation(
       const arma::Col<double>& parameter) const override {
       parameterHistory_.push_back(parameter);

      return objectiveValues_.at(ValueIndex_);
    }

    double getSoftConstraintsValueImplementation(
       const arma::Col<double>& parameter) const override {

      return softConstraintsValues_.at(ValueIndex_);
    }

    std::string to_string() const noexcept {
      return "TestHillClimbing";
    }
};

decltype(TestHillClimbingProblem::parameterHistory_) TestHillClimbingProblem::parameterHistory_;

TEST_CASE("Hill climbing", "") {

   // Values OptimisationProblem
   double dimension = 4;

   // Values OptimisationAlgorithm
   arma::mat velocities;
   velocities.load(testDirectory.string() + dataPath_ + "velocities.mat");

   // Load Expected
   arma::Mat<double> expectedParameterHistory;
   expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected.mat");


  std::shared_ptr<TestHillClimbingProblem> testHillClimbingProblem(new TestHillClimbingProblem(dimension));

  arma::Col<double> upperBounds;
  if(upperBounds.quiet_load(testDirectory.string() + dataPath_ + "upperBounds.mat")) {
    testHillClimbingProblem->setUpperBounds(upperBounds);
  };
  arma::Col<double> lowerBounds;
  if(upperBounds.quiet_load(testDirectory.string() + dataPath_ + "lowerBounds.mat")) {
    testHillClimbingProblem->setLowerBounds(lowerBounds);
  }

  TestHillClimbing testHillClimbing(testHillClimbingProblem);
  arma::Col<double> initialParameter;
  if(initialParameter.quiet_load(testDirectory.string() + dataPath_ + "initialParameter.mat")){
    testHillClimbing.setInitialParameter(initialParameter);
  };
  arma::Col<double> maximalStepSize;
  if(maximalStepSize.quiet_load(testDirectory.string() + dataPath_ + "maximalStepSize.mat")){
    testHillClimbing.setMaximalStepSize(maximalStepSize);
  };

  testHillClimbing.optimise();

  std::vector<arma::Col<double>> actualParameterHistory = testHillClimbingProblem->getParameterHistory();

  for(std::size_t n = 0; n < expectedParameterHistory.n_cols; ++n) {
    arma::Col<double> expectedParameter = expectedParameterHistory.col(n);
    arma::Col<double> actualParameter = actualParameterHistory.at(n);

    for (std::size_t k = 0; k < expectedParameter.n_elem; ++k) {
      CHECK(actualParameter.at(k) == Approx(expectedParameter.at(k)));
    }
  }
}

