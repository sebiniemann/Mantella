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
static std::string dataPath_ = "/data/optimisationAlgorithm/trajectoryBasedAlgrorithm/hookeJeeves/";

class TestHookeJeeves : public mant::HookeJeevesAlgorithm {
  public:
    TestHookeJeeves(
        const std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem)
      : mant::HookeJeevesAlgorithm(optimisationProblem){
    }
};

class TestHookeJeevesProblem : public mant::OptimisationProblem<double> {
  public:
    TestHookeJeevesProblem(
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
      return "TestHookeJeeves";
    }
};

decltype(TestHookeJeevesProblem::parameterHistory_) TestHookeJeevesProblem::parameterHistory_;
decltype(TestHookeJeevesProblem::softConstraintsValuesIndex_) TestHookeJeevesProblem::softConstraintsValuesIndex_;
decltype(TestHookeJeevesProblem::objectiveValuesIndex_) TestHookeJeevesProblem::objectiveValuesIndex_;

TEST_CASE("Hooke Jeeves", "") {

   // Set OptimisationProblem values
   arma::Col<double> upperBounds;
   upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds[2.1].mat"); //the parametre is optional

   arma::Col<double> lowerBounds;
   lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds[2.1].mat"); //the parametre is optional

   arma::Col<double> objectiveValues;
   objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues[2.1].mat");

   arma::Col<double> softConstraintsValues;
   softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues[2.1].mat");

   // Init OptimisationProblem
   std::shared_ptr<TestHookeJeevesProblem> testHookeJeevesProblem(new TestHookeJeevesProblem(upperBounds.n_elem));
   testHookeJeevesProblem->setUpperBounds(upperBounds);
   testHookeJeevesProblem->setLowerBounds(lowerBounds);

   testHookeJeevesProblem->setAcceptableObjectiveValue(-500.0);
   testHookeJeevesProblem->setObjectiveValues(objectiveValues);
   testHookeJeevesProblem->setSoftConstraintsValues(softConstraintsValues);

   // Set OptimisationAlgorithm values
   arma::Col<double> stepSizeDecrease;
   stepSizeDecrease.load(testDirectory.string() + dataPath_ + "stepSizeDecrease[2.1].mat");

   arma::Col<double> stepSize;
   stepSize.load(testDirectory.string() + dataPath_ + "stepSize[2.1].mat");

   arma::Col<double> initialParameter;
   initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter[2.1].mat"); //the parametre is optional

   // Init OptimisationAlgorithm
   TestHookeJeeves testHookeJeeves(testHookeJeevesProblem);
   testHookeJeeves.setStepSizeDecrease(stepSizeDecrease);
   testHookeJeeves.setInitialStepSize(stepSize);
   testHookeJeeves.setInitialParameter(initialParameter);

   // Set Expected values
   arma::Mat<double> expectedParameterHistory;
   expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected[2.1].mat");

   // Run HookeJeeves algorithm
  testHookeJeeves.optimise();
  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testHookeJeevesProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);


}

TEST_CASE("HookeJeeves Test state", "") {

   // Set OptimisationProblem values
   arma::Col<double> upperBounds;
   upperBounds.load(testDirectory.string() + dataPath_ + "upperBounds[2.2].mat"); //the parametre is optional

   arma::Col<double> lowerBounds;
   lowerBounds.load(testDirectory.string() + dataPath_ + "lowerBounds[2.2].mat"); //the parametre is optional

   arma::Col<double> objectiveValues;
   objectiveValues.load(testDirectory.string() + dataPath_ + "objectiveValues[2.2].mat");

   arma::Col<double> softConstraintsValues;
   softConstraintsValues.load(testDirectory.string() + dataPath_ + "softConstraintsValues[2.2].mat");

   // Init OptimisationProblem
   std::shared_ptr<TestHookeJeevesProblem> testHookeJeevesProblem(new TestHookeJeevesProblem(upperBounds.n_elem));
   testHookeJeevesProblem->setUpperBounds(upperBounds);
   testHookeJeevesProblem->setLowerBounds(lowerBounds);

   testHookeJeevesProblem->setAcceptableObjectiveValue(-500.0);
   testHookeJeevesProblem->setObjectiveValues(objectiveValues);
   testHookeJeevesProblem->setSoftConstraintsValues(softConstraintsValues);

   // Set OptimisationAlgorithm values
   arma::Col<double> stepSizeDecrease;
   stepSizeDecrease.load(testDirectory.string() + dataPath_ + "stepSizeDecrease[2.1].mat");

   arma::Col<double> stepSize;
   stepSize.load(testDirectory.string() + dataPath_ + "stepSize[2.1].mat");

   arma::Col<double> initialParameter;
   initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter[2.2].mat"); //the parametre is optional

   // Init OptimisationAlgorithm
   TestHookeJeeves testHookeJeeves(testHookeJeevesProblem);
   testHookeJeeves.setStepSizeDecrease(stepSizeDecrease);
   testHookeJeeves.setInitialStepSize(stepSize);
   testHookeJeeves.setInitialParameter(initialParameter);

   // Set Expected values
   arma::Mat<double> expectedParameterHistory;
   expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected[2.2].mat");

   // Run HookeJeeves algorithm
  testHookeJeeves.optimise();
  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testHookeJeevesProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);


}

TEST_CASE("HookeJeeves Test Maximalstepsize", "") {

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
  std::shared_ptr<TestHookeJeevesProblem> testHookeJeevesProblem(new TestHookeJeevesProblem(upperBounds.n_elem));
  testHookeJeevesProblem->setUpperBounds(upperBounds);
  testHookeJeevesProblem->setLowerBounds(lowerBounds);
  testHookeJeevesProblem->setObjectiveValues(objectiveValues);
  testHookeJeevesProblem->setSoftConstraintsValues(softConstraintsValues);
  testHookeJeevesProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::Col<double> stepSizeDecrease;
  stepSizeDecrease.load(testDirectory.string() + dataPath_ + "stepSizeDecrease[2.1].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestHookeJeeves testHookeJeeves(testHookeJeevesProblem);
  testHookeJeeves.setStepSizeDecrease(stepSizeDecrease);
  testHookeJeeves.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run HookeJeeves algorithm
  testHookeJeeves.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testHookeJeevesProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("HookeJeeves Check initialParameter at each limit", "") {

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
   std::shared_ptr<TestHookeJeevesProblem> testHookeJeevesProblem(new TestHookeJeevesProblem(upperBounds.n_elem));
   testHookeJeevesProblem->setUpperBounds(upperBounds);
   testHookeJeevesProblem->setLowerBounds(lowerBounds);
   testHookeJeevesProblem->setObjectiveValues(objectiveValues);
   testHookeJeevesProblem->setSoftConstraintsValues(softConstraintsValues);
   testHookeJeevesProblem->setAcceptableObjectiveValue(10.0);

   // Set OptimisationAlgorithm values
   arma::Col<double> stepSizeDecrease;
   stepSizeDecrease.load(testDirectory.string() + dataPath_ + "stepSizeDecrease[2.1].mat");

   arma::Col<double> initialParameter;
   initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

   // Init OptimisationAlgorithm
   TestHookeJeeves testHookeJeeves(testHookeJeevesProblem);
   testHookeJeeves.setStepSizeDecrease(stepSizeDecrease);
   testHookeJeeves.setInitialParameter(initialParameter);

   // Set Expected values
   arma::Mat<double> expectedParameterHistory;
   expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

   // Run Simulated Annealing algorithm
   testHookeJeeves.optimise();

   // Comparing of candidateParameters
   std::vector<arma::Col<double>> actualParameterHistory = testHookeJeevesProblem->getParameterHistory();
   trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("HookeJeeves Check initialParameter at one limit", "") {

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
  std::shared_ptr<TestHookeJeevesProblem> testHookeJeevesProblem(new TestHookeJeevesProblem(upperBounds.n_elem));
  testHookeJeevesProblem->setUpperBounds(upperBounds);
  testHookeJeevesProblem->setLowerBounds(lowerBounds);
  testHookeJeevesProblem->setObjectiveValues(objectiveValues);
  testHookeJeevesProblem->setSoftConstraintsValues(softConstraintsValues);
  testHookeJeevesProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::Col<double> stepSizeDecrease;
  stepSizeDecrease.load(testDirectory.string() + dataPath_ + "stepSizeDecrease[2.1].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestHookeJeeves testHookeJeeves(testHookeJeevesProblem);
  testHookeJeeves.setStepSizeDecrease(stepSizeDecrease);
  testHookeJeeves.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run HookeJeeves algorithm
  testHookeJeeves.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testHookeJeevesProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("HookeJeeves Check initialParameter in-range", "") {

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
  std::shared_ptr<TestHookeJeevesProblem> testHookeJeevesProblem(new TestHookeJeevesProblem(upperBounds.n_elem));
  testHookeJeevesProblem->setUpperBounds(upperBounds);
  testHookeJeevesProblem->setLowerBounds(lowerBounds);
  testHookeJeevesProblem->setObjectiveValues(objectiveValues);
  testHookeJeevesProblem->setSoftConstraintsValues(softConstraintsValues);
  testHookeJeevesProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::Col<double> stepSizeDecrease;
  stepSizeDecrease.load(testDirectory.string() + dataPath_ + "stepSizeDecrease[2.1].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestHookeJeeves testHookeJeeves(testHookeJeevesProblem);
  testHookeJeeves.setStepSizeDecrease(stepSizeDecrease);
  testHookeJeeves.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run HookeJeeves algorithm
  testHookeJeeves.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testHookeJeevesProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("HookeJeeves Check initialParameter out of range maximal limit", "") {

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
  std::shared_ptr<TestHookeJeevesProblem> testHookeJeevesProblem(new TestHookeJeevesProblem(upperBounds.n_elem));
  testHookeJeevesProblem->setUpperBounds(upperBounds);
  testHookeJeevesProblem->setLowerBounds(lowerBounds);
  testHookeJeevesProblem->setObjectiveValues(objectiveValues);
  testHookeJeevesProblem->setSoftConstraintsValues(softConstraintsValues);
  testHookeJeevesProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::Col<double> stepSizeDecrease;
  stepSizeDecrease.load(testDirectory.string() + dataPath_ + "stepSizeDecrease[2.1].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestHookeJeeves testHookeJeeves(testHookeJeevesProblem);
  testHookeJeeves.setStepSizeDecrease(stepSizeDecrease);
  testHookeJeeves.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run Simulated Annealing algorithm
  testHookeJeeves.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testHookeJeevesProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }

TEST_CASE("HookeJeeves Check initialParameter out of range arbitrary values", "") {

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
  std::shared_ptr<TestHookeJeevesProblem> testHookeJeevesProblem(new TestHookeJeevesProblem(upperBounds.n_elem));
  testHookeJeevesProblem->setUpperBounds(upperBounds);
  testHookeJeevesProblem->setLowerBounds(lowerBounds);
  testHookeJeevesProblem->setObjectiveValues(objectiveValues);
  testHookeJeevesProblem->setSoftConstraintsValues(softConstraintsValues);
  testHookeJeevesProblem->setAcceptableObjectiveValue(10.0);

  // Set OptimisationAlgorithm values
  arma::Col<double> stepSizeDecrease;
  stepSizeDecrease.load(testDirectory.string() + dataPath_ + "stepSizeDecrease[2.1].mat");

  arma::Col<double> initialParameter;
  initialParameter.load(testDirectory.string() + dataPath_ + "initialParameter["+ expectedName +"].mat");

  // Init OptimisationAlgorithm
  TestHookeJeeves testHookeJeeves(testHookeJeevesProblem);
  testHookeJeeves.setStepSizeDecrease(stepSizeDecrease);
  testHookeJeeves.setInitialParameter(initialParameter);

  // Set Expected values
  arma::Mat<double> expectedParameterHistory;
  expectedParameterHistory.load(testDirectory.string() + dataPath_ + "expected["+ expectedName +"].mat");

  // Run HookeJeeves algorithm
  testHookeJeeves.optimise();

  // Comparing of candidateParameters
  std::vector<arma::Col<double>> actualParameterHistory = testHookeJeevesProblem->getParameterHistory();
  trajectoryBasedAlgorithmHelper::compareResults(actualParameterHistory,expectedParameterHistory);
  }


