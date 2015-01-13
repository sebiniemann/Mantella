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
    }

    arma::Col<double> getVelocity() override {
      std::cout << "velocityIndex_: " << velocityIndex_ << std::endl;
      return velocities_.col(velocityIndex_++);
    }

    void setVelocitys(arma::mat velocities){
      velocities_ = velocities;
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
    }


    std::vector<arma::Col<double>> getParameterHistory() const noexcept {
      return parameterHistory_;
    }

    void setObjectiveValues(arma::Col<double> objectiveValues){
      objectiveValues_ = objectiveValues;
    }

    void setSoftConstraintsValues(arma::Col<double> softConstraintsValues){
      softConstraintsValues_ = softConstraintsValues;
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

   // Set OptimisationProblem values
   arma::Col<double> upperBounds;
   upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds.mat"); //the parametre is optional

   arma::Col<double> lowerBounds;
   lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds.mat"); //the parametre is optional

   arma::Col<double> objectiveValues;
   objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues[2.0].mat");

   arma::Col<double> softConstraintsValues;
   softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues[2.0].mat");

   // Init OptimisationProblem
   std::shared_ptr<TestHillClimbingProblem> testHillClimbingProblem(new TestHillClimbingProblem(upperBounds.n_elem));
   testHillClimbingProblem->setUpperBounds(upperBounds);
   testHillClimbingProblem->setLowerBounds(lowerBounds);
   testHillClimbingProblem->setAcceptableObjectiveValue(-500.0);
   testHillClimbingProblem->setObjectiveValues(objectiveValues);
   testHillClimbingProblem->setSoftConstraintsValues(softConstraintsValues);

   // Set OptimisationAlgorithm values
   arma::mat velocities;
   velocities.load(testDirectory.string() + dataPath_ + "velocities[2.0].mat");

   arma::Col<double> initialParameter;
   initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter[null].mat"); //the parametre is optional

   arma::Col<double> maximalStepSize;
   maximalStepSize.load(testDirectory.string() + dataPath_ + "maximalStepSize[2.1].mat"); //the parametre is optional

   // Init OptimisationAlgorithm
   TestHillClimbing testHillClimbing(testHillClimbingProblem);
   testHillClimbing.setVelocitys(velocities);
   testHillClimbing.setInitialParameter(initialParameter);
   testHillClimbing.setMaximalStepSize(maximalStepSize);

   // Set Expected values
   arma::Mat<double> expectedParameterHistory;
   expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected[2.1].mat");

   // Run hill climbing algorithm
  testHillClimbing.optimise();
  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testHillClimbingProblem->getParameterHistory();

  std::cout << "expectedParameterHistory.n_cols: " << expectedParameterHistory.n_cols << std::endl;
  for(std::size_t n = 0; n < expectedParameterHistory.n_cols; ++n) {
    arma::Col<double> expectedParameter = expectedParameterHistory.col(n);
    arma::Col<double> actualParameter = actualParameterHistory.at(n);

    for (std::size_t k = 0; k < expectedParameter.n_elem; ++k) {
      CHECK(actualParameter.at(k) == Approx(expectedParameter.at(k)));
    }
  }
}

TEST_CASE("Hill climbing (init Test)", "") {

   // Set OptimisationProblem values
   arma::Col<double> upperBounds;
   upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds.mat"); //the parametre is optional

   arma::Col<double> lowerBounds;
   lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds.mat"); //the parametre is optional

   arma::Col<double> objectiveValues;
   objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues[1.0].mat");

   arma::Col<double> softConstraintsValues;
   softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues[1.0].mat");

   // Init OptimisationProblem
   std::shared_ptr<TestHillClimbingProblem> testHillClimbingProblem(new TestHillClimbingProblem(upperBounds.n_elem));
   testHillClimbingProblem->setUpperBounds(upperBounds);
   testHillClimbingProblem->setLowerBounds(lowerBounds);
   testHillClimbingProblem->setObjectiveValues(objectiveValues);
   testHillClimbingProblem->setSoftConstraintsValues(softConstraintsValues);

   // Set OptimisationAlgorithm values
   arma::mat velocities;
   velocities.load(testDirectory.string() + dataPath_ + "velocities[1.0].mat");

   // Init OptimisationAlgorithm
   TestHillClimbing testHillClimbing(testHillClimbingProblem);
   testHillClimbing.setVelocitys(velocities);

   arma::Mat<double> expectedParameterHistory;

  SECTION("Check MaximalStepSize"){
    arma::Col<double> initialParameter;
    initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter[null].mat"); //the parametre is optional

    //arma::Col<double> maximalStepSize;
    //maximalStepSize.load(testDirectory.string() + dataPath_ + "maximalStepSize[1.1].mat"); //the parametre is optional

    expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected[1.1].mat");

    testHillClimbing.setInitialParameter(initialParameter);
    //testHillClimbing.setMaximalStepSize(maximalStepSize);
    testHillClimbing.optimise();

    // Comparing of candidateParameters
    std::vector<arma::Col<double>> actualParameterHistory = testHillClimbingProblem->getParameterHistory();

    for(std::size_t n = 0; n < expectedParameterHistory.n_cols; ++n) {
      arma::Col<double> expectedParameter = expectedParameterHistory.col(n);
      arma::Col<double> actualParameter = actualParameterHistory.at(n);

      for (std::size_t k = 0; k < expectedParameter.n_elem; ++k) {
        CHECK(actualParameter.at(k) == Approx(expectedParameter.at(k)));
      }
    }

  }

}
