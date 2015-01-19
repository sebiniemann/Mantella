// Catch
#include <catch.hpp>

// C++ Standard Library
#include <memory>
#include <iostream>
#include <cmath>

// Boost
#include <boost/filesystem.hpp>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

// Helper
#include "trajectoryBasedAlgorithmHelper.cpp"

extern boost::filesystem::path testDirectory;
static std::string dataPath_ = "/data/optimisationAlgorithm/trajectoryBasedAlgrorithm/simulatedAnnealing/";

class simmulatedAnnealing{
void compareResults(std::vector<arma::Col<double>> actualHistory,arma::Mat<double> expected){
    for(std::size_t n = 0; n < expected.n_cols; ++n) {
      arma::Col<double> expectedParameter = expected.col(n);
      arma::Col<double> actualParameter = actualHistory.at(n);

      for (std::size_t k = 0; k < expectedParameter.n_elem; ++k) {
        if(std::isfinite(expectedParameter.at(k))) {
          CHECK(actualParameter.at(k) == Approx(expectedParameter.at(k)));
        } else {
          CHECK(actualParameter.at(k) == expectedParameter.at(k));
        }
      }
    }
}
};

class TestSimulatedAnnealing : public mant::SimulatedAnnealing<double, mant::EuclideanDistance> {
  public:
    TestSimulatedAnnealing(
        const std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem)
      : mant::SimulatedAnnealing<double, mant::EuclideanDistance>(optimisationProblem),
        velocityIndex_(0),
        statesIndex_(0){
    }

    arma::Col<double> getVelocity() noexcept {
      return velocities_.col(velocityIndex_++);
    }

    bool isAcceptableState(const double& candidateObjectiveValue) noexcept {
      if(states_.at(statesIndex_++)==1){
        return true;
      }
      return false;
    }

    void setVelocitys(arma::mat velocities){
      velocities_ = velocities;
    }

    void setStates(arma::mat states){
      states_ = states;
    }

  protected:
    unsigned int velocityIndex_;
    unsigned int statesIndex_;
    arma::mat velocities_;
    arma::mat states_;
};

class TestSimulatedAnnealingProblem : public mant::OptimisationProblem<double> {
  public:
    TestSimulatedAnnealingProblem(
        const unsigned int numberOfDimensions)
      : mant::OptimisationProblem<double>(numberOfDimensions) {
      softConstraintsValuesIndex_ = 0;
      objectiveValuesIndex_ = 0;
      parameterHistory_.clear();
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
    static unsigned int softConstraintsValuesIndex_;
    static unsigned int objectiveValuesIndex_;
    arma::Col<double> objectiveValues_;
    arma::Col<double> softConstraintsValues_;

    static std::vector<arma::Col<double>> parameterHistory_;

    double getSoftConstraintsValueImplementation(
       const arma::Col<double>& parameter) const noexcept override {

      return softConstraintsValues_.at(softConstraintsValuesIndex_++);
    }

    double getObjectiveValueImplementation(
       const arma::Col<double>& parameter) const noexcept override {
       parameterHistory_.push_back(parameter);

      return objectiveValues_.at(objectiveValuesIndex_++);
    }

    std::string to_string() const noexcept {
      return "TestSimulatedAnnealing";
    }
};

decltype(TestSimulatedAnnealingProblem::parameterHistory_) TestSimulatedAnnealingProblem::parameterHistory_;
decltype(TestSimulatedAnnealingProblem::softConstraintsValuesIndex_) TestSimulatedAnnealingProblem::softConstraintsValuesIndex_;
decltype(TestSimulatedAnnealingProblem::objectiveValuesIndex_) TestSimulatedAnnealingProblem::objectiveValuesIndex_;

TEST_CASE("Simulated annealing", "") {

   // Set OptimisationProblem values
   arma::Col<double> upperBounds;
   upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds[2.1].mat"); // The parameter is optional

   arma::Col<double> lowerBounds;
   lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds[2.1].mat"); // The parameter is optional

   arma::Col<double> objectiveValues;
   objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues[2.1].mat");

   arma::Col<double> softConstraintsValues;
   softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues[2.1].mat");

   // Init OptimisationProblem
   std::shared_ptr<TestSimulatedAnnealingProblem> testSimulatedAnnealingProblem(new TestSimulatedAnnealingProblem(upperBounds.n_elem));
   testSimulatedAnnealingProblem->setUpperBounds(upperBounds);
   testSimulatedAnnealingProblem->setLowerBounds(lowerBounds);

   testSimulatedAnnealingProblem->setAcceptableObjectiveValue(-500.0);
   testSimulatedAnnealingProblem->setObjectiveValues(objectiveValues);
   testSimulatedAnnealingProblem->setSoftConstraintsValues(softConstraintsValues);

   // Set OptimisationAlgorithm values
   arma::mat velocities;
   velocities.load(testDirectory.string() + dataPath_ + "velocities[2.1].mat");

   arma::mat states;
   states.quiet_load(testDirectory.string() + dataPath_ + "acceptableStates[2.1].mat");

   arma::Col<double> initialParameter;
   initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter[2.1].mat"); // The parameter is optional

   arma::Col<double> maximalStepSize;
   maximalStepSize.load(testDirectory.string() + dataPath_ + "maximalStepSize[2.1].mat"); // The parameter is optional

   // Init OptimisationAlgorithm
   TestSimulatedAnnealing testSimulatedAnnealing(testSimulatedAnnealingProblem);
   testSimulatedAnnealing.setVelocitys(velocities);
   testSimulatedAnnealing.setStates(states);
   testSimulatedAnnealing.setInitialParameter(initialParameter);
   testSimulatedAnnealing.setMaximalStepSize(maximalStepSize);

   // Set Expected values
   arma::Mat<double> expectedParameterHistory;
   expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected[2.1].mat");

   // Run SimulatedAnnealing algorithm
  testSimulatedAnnealing.optimise();
  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testSimulatedAnnealingProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);


}

TEST_CASE("SimulatedAnnealing Test state", "") {

   // Set OptimisationProblem values
   arma::Col<double> upperBounds;
   upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds[2.2].mat"); // The parameter is optional

   arma::Col<double> lowerBounds;
   lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds[2.2].mat"); // The parameter is optional

   arma::Col<double> objectiveValues;
   objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues[2.2].mat");

   arma::Col<double> softConstraintsValues;
   softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues[2.2].mat");

   // Init OptimisationProblem
   std::shared_ptr<TestSimulatedAnnealingProblem> testSimulatedAnnealingProblem(new TestSimulatedAnnealingProblem(upperBounds.n_elem));
   testSimulatedAnnealingProblem->setUpperBounds(upperBounds);
   testSimulatedAnnealingProblem->setLowerBounds(lowerBounds);

   testSimulatedAnnealingProblem->setAcceptableObjectiveValue(-500.0);
   testSimulatedAnnealingProblem->setObjectiveValues(objectiveValues);
   testSimulatedAnnealingProblem->setSoftConstraintsValues(softConstraintsValues);

   // Set OptimisationAlgorithm values
   arma::mat velocities;
   velocities.load(testDirectory.string() + dataPath_ + "velocities[2.2].mat");

   arma::mat states;
   states.quiet_load(testDirectory.string() + dataPath_ + "acceptableStates[2.2].mat");

   arma::Col<double> initialParameter;
   initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter[2.2].mat"); // The parameter is optional

   arma::Col<double> maximalStepSize;
   maximalStepSize.load(testDirectory.string() + dataPath_ + "maximalStepSize[2.2].mat"); // The parameter is optional

   // Init OptimisationAlgorithm
   TestSimulatedAnnealing testSimulatedAnnealing(testSimulatedAnnealingProblem);
   testSimulatedAnnealing.setVelocitys(velocities);
   testSimulatedAnnealing.setStates(states);
   testSimulatedAnnealing.setInitialParameter(initialParameter);
   testSimulatedAnnealing.setMaximalStepSize(maximalStepSize);

   // Set Expected values
   arma::Mat<double> expectedParameterHistory;
   expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected[2.2].mat");

   // Run SimulatedAnnealing algorithm
  testSimulatedAnnealing.optimise();
  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testSimulatedAnnealingProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);


}

TEST_CASE("SimulatedAnnealing Test Maximalstepsize", "") {

  // name for the expected data
  std::string expectedName = "1.1";

  //Set OptimisationProblem values
  arma::Col<double> upperBounds;
  upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds["+ expectedName +"].mat");

  arma::Col<double> lowerBounds;
  lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds["+ expectedName +"].mat");

  arma::Col<double> objectiveValues;
  objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues["+ expectedName +"].mat");

  arma::Col<double> softConstraintsValues;
  softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues["+ expectedName +"].mat");

  // Init OptimisationProblem
  std::shared_ptr<TestSimulatedAnnealingProblem> testSimulatedAnnealingProblem(new TestSimulatedAnnealingProblem(upperBounds.n_elem));
  testSimulatedAnnealingProblem->setUpperBounds(upperBounds);
  testSimulatedAnnealingProblem->setLowerBounds(lowerBounds);
  testSimulatedAnnealingProblem->setObjectiveValues(objectiveValues);
  testSimulatedAnnealingProblem->setSoftConstraintsValues(softConstraintsValues);
  testSimulatedAnnealingProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::mat velocities;
  velocities.load(testDirectory.string() + dataPath_ + "velocities["+ expectedName +"].mat");

  arma::mat states;
  states.quiet_load(testDirectory.string() + dataPath_ + "acceptableStates["+ expectedName +"].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestSimulatedAnnealing testSimulatedAnnealing(testSimulatedAnnealingProblem);
  testSimulatedAnnealing.setVelocitys(velocities);
  testSimulatedAnnealing.setStates(states);
  testSimulatedAnnealing.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run SimulatedAnnealing algorithm
  testSimulatedAnnealing.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testSimulatedAnnealingProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("SimulatedAnnealing Check initialParameter at each limit", "") {

   // name for the expected data
   std::string expectedName = "1.2";

   //Set OptimisationProblem values
   arma::Col<double> upperBounds;
   upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds["+ expectedName +"].mat");

   arma::Col<double> lowerBounds;
   lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds["+ expectedName +"].mat");

   arma::Col<double> objectiveValues;
   objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues["+ expectedName +"].mat");

   arma::Col<double> softConstraintsValues;
   softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues["+ expectedName +"].mat");

   // Init OptimisationProblem
   std::shared_ptr<TestSimulatedAnnealingProblem> testSimulatedAnnealingProblem(new TestSimulatedAnnealingProblem(upperBounds.n_elem));
   testSimulatedAnnealingProblem->setUpperBounds(upperBounds);
   testSimulatedAnnealingProblem->setLowerBounds(lowerBounds);
   testSimulatedAnnealingProblem->setObjectiveValues(objectiveValues);
   testSimulatedAnnealingProblem->setSoftConstraintsValues(softConstraintsValues);
   testSimulatedAnnealingProblem->setAcceptableObjectiveValue(10.0);

   // Set OptimisationAlgorithm values
   arma::mat velocities;
   velocities.load(testDirectory.string() + dataPath_ + "velocities["+ expectedName +"].mat");

   arma::mat states;
   states.quiet_load(testDirectory.string() + dataPath_ + "acceptableStates["+ expectedName +"].mat");

   arma::Col<double> initialParameter;
   initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

   // Init OptimisationAlgorithm
   TestSimulatedAnnealing testSimulatedAnnealing(testSimulatedAnnealingProblem);
   testSimulatedAnnealing.setVelocitys(velocities);
   testSimulatedAnnealing.setStates(states);
   testSimulatedAnnealing.setInitialParameter(initialParameter);

   // Set Expected values
   arma::Mat<double> expectedParameterHistory;
   expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

   // Run Simulated Annealing algorithm
   testSimulatedAnnealing.optimise();

   // Comparing of candidateParameters
   std::vector<arma::Col<double>> actualParameterHistory = testSimulatedAnnealingProblem->getParameterHistory();
   trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("SimulatedAnnealing Check initialParameter at one limit", "") {

  // name for the expected data
  std::string expectedName = "1.3";

  //Set OptimisationProblem values
  arma::Col<double> upperBounds;
  upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds["+ expectedName +"].mat");

  arma::Col<double> lowerBounds;
  lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds["+ expectedName +"].mat");

  arma::Col<double> objectiveValues;
  objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues["+ expectedName +"].mat");

  arma::Col<double> softConstraintsValues;
  softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues["+ expectedName +"].mat");

  // Init OptimisationProblem
  std::shared_ptr<TestSimulatedAnnealingProblem> testSimulatedAnnealingProblem(new TestSimulatedAnnealingProblem(upperBounds.n_elem));
  testSimulatedAnnealingProblem->setUpperBounds(upperBounds);
  testSimulatedAnnealingProblem->setLowerBounds(lowerBounds);
  testSimulatedAnnealingProblem->setObjectiveValues(objectiveValues);
  testSimulatedAnnealingProblem->setSoftConstraintsValues(softConstraintsValues);
  testSimulatedAnnealingProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::mat velocities;
  velocities.load(testDirectory.string() + dataPath_ + "velocities["+ expectedName +"].mat");

  arma::mat states;
  states.quiet_load(testDirectory.string() + dataPath_ + "acceptableStates["+ expectedName +"].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestSimulatedAnnealing testSimulatedAnnealing(testSimulatedAnnealingProblem);
  testSimulatedAnnealing.setVelocitys(velocities);
  testSimulatedAnnealing.setStates(states);
  testSimulatedAnnealing.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run SimulatedAnnealing algorithm
  testSimulatedAnnealing.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testSimulatedAnnealingProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("SimulatedAnnealing Check initialParameter in-range", "") {

  // name for the expected data
  std::string expectedName = "1.4";

  //Set OptimisationProblem values
  arma::Col<double> upperBounds;
  upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds["+ expectedName +"].mat");

  arma::Col<double> lowerBounds;
  lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds["+ expectedName +"].mat");

  arma::Col<double> objectiveValues;
  objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues["+ expectedName +"].mat");

  arma::Col<double> softConstraintsValues;
  softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues["+ expectedName +"].mat");

  // Init OptimisationProblem
  std::shared_ptr<TestSimulatedAnnealingProblem> testSimulatedAnnealingProblem(new TestSimulatedAnnealingProblem(upperBounds.n_elem));
  testSimulatedAnnealingProblem->setUpperBounds(upperBounds);
  testSimulatedAnnealingProblem->setLowerBounds(lowerBounds);
  testSimulatedAnnealingProblem->setObjectiveValues(objectiveValues);
  testSimulatedAnnealingProblem->setSoftConstraintsValues(softConstraintsValues);
  testSimulatedAnnealingProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::mat velocities;
  velocities.load(testDirectory.string() + dataPath_ + "velocities["+ expectedName +"].mat");

  arma::mat states;
  states.quiet_load(testDirectory.string() + dataPath_ + "acceptableStates["+ expectedName +"].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestSimulatedAnnealing testSimulatedAnnealing(testSimulatedAnnealingProblem);
  testSimulatedAnnealing.setVelocitys(velocities);
  testSimulatedAnnealing.setStates(states);
  testSimulatedAnnealing.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run SimulatedAnnealing algorithm
  testSimulatedAnnealing.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testSimulatedAnnealingProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("SimulatedAnnealing Check initialParameter out of range maximal limit", "") {

  // name for the expected data
  std::string expectedName = "1.5";

  //Set OptimisationProblem values
  arma::Col<double> upperBounds;
  upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds["+ expectedName +"].mat");

  arma::Col<double> lowerBounds;
  lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds["+ expectedName +"].mat");

  arma::Col<double> objectiveValues;
  objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues["+ expectedName +"].mat");

  arma::Col<double> softConstraintsValues;
  softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues["+ expectedName +"].mat");

  // Init OptimisationProblem
  std::shared_ptr<TestSimulatedAnnealingProblem> testSimulatedAnnealingProblem(new TestSimulatedAnnealingProblem(upperBounds.n_elem));
  testSimulatedAnnealingProblem->setUpperBounds(upperBounds);
  testSimulatedAnnealingProblem->setLowerBounds(lowerBounds);
  testSimulatedAnnealingProblem->setObjectiveValues(objectiveValues);
  testSimulatedAnnealingProblem->setSoftConstraintsValues(softConstraintsValues);
  testSimulatedAnnealingProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::mat velocities;
  velocities.load(testDirectory.string() + dataPath_ + "velocities["+ expectedName +"].mat");

  arma::mat states;
  states.quiet_load(testDirectory.string() + dataPath_ + "acceptableStates["+ expectedName +"].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestSimulatedAnnealing testSimulatedAnnealing(testSimulatedAnnealingProblem);
  testSimulatedAnnealing.setVelocitys(velocities);
  testSimulatedAnnealing.setStates(states);
  testSimulatedAnnealing.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run Simulated Annealing algorithm
  testSimulatedAnnealing.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testSimulatedAnnealingProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("SimulatedAnnealing Check initialParameter out of range arbitrary values", "") {

  // name for the expected data
  std::string expectedName = "1.6";

  //Set OptimisationProblem values
  arma::Col<double> upperBounds;
  upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds["+ expectedName +"].mat");

  arma::Col<double> lowerBounds;
  lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds["+ expectedName +"].mat");

  arma::Col<double> objectiveValues;
  objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues["+ expectedName +"].mat");

  arma::Col<double> softConstraintsValues;
  softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues["+ expectedName +"].mat");

  // Init OptimisationProblem
  std::shared_ptr<TestSimulatedAnnealingProblem> testSimulatedAnnealingProblem(new TestSimulatedAnnealingProblem(upperBounds.n_elem));
  testSimulatedAnnealingProblem->setUpperBounds(upperBounds);
  testSimulatedAnnealingProblem->setLowerBounds(lowerBounds);
  testSimulatedAnnealingProblem->setObjectiveValues(objectiveValues);
  testSimulatedAnnealingProblem->setSoftConstraintsValues(softConstraintsValues);
  testSimulatedAnnealingProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::mat velocities;
  velocities.load(testDirectory.string() + dataPath_ + "velocities["+ expectedName +"].mat");

  arma::mat states;
  states.quiet_load(testDirectory.string() + dataPath_ + "acceptableStates["+ expectedName +"].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestSimulatedAnnealing testSimulatedAnnealing(testSimulatedAnnealingProblem);
  testSimulatedAnnealing.setVelocitys(velocities);
  testSimulatedAnnealing.setStates(states);
  testSimulatedAnnealing.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run SimulatedAnnealing algorithm
  testSimulatedAnnealing.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testSimulatedAnnealingProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }


