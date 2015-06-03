// Catch
#include <catch.hpp>
#include "helper.hpp"

// C++ Standard Library
#include <limits>
#include <map>

// Mantella
#include <mantella>

class TestOptimisationProblem : public mant::OptimisationProblem<double> {
  public:
    TestOptimisationProblem(
        const unsigned int numberOfDimensions)
      : mant::OptimisationProblem<double>(numberOfDimensions) {}

    std::string toString() const noexcept override {
      return "test_optimisation_problem";
    }

  protected:
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept override {
      double defect = 0;
      for (unsigned int i = 0; i < parameter.n_elem; i++) {
        for (unsigned int j = i; j < parameter.n_elem; j++) {
          if (parameter.at(i) > parameter.at(j)) {
            defect++;
          }
        };
      };
      return (defect + arma::sum(parameter) + getNumberOfEvaluations());
    }
};

class TestSoftConstraintsOptimisationProblem : public mant::OptimisationProblem<double> {
  public:
    TestSoftConstraintsOptimisationProblem(
        const unsigned int numberOfDimensions)
      : mant::OptimisationProblem<double>(numberOfDimensions) {}

    std::string toString() const noexcept override {
      return "test_soft_constraints_optimisation_problem";
    }

  protected:
    double getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept override {
      double defect = 0;
      for (unsigned int i = 0; i < parameter.n_elem; i++) {
        for (unsigned int j = i; j < parameter.n_elem; j++) {
          if (parameter.at(i) > parameter.at(j)) {
            defect++;
          }
        };
      };
      return (defect + arma::sum(parameter));
    }

    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept override {
      double defect = 0;
      for (unsigned int i = 0; i < parameter.n_elem; i++) {
        for (unsigned int j = i; j < parameter.n_elem; j++) {
          if (parameter.at(i) > parameter.at(j)) {
            defect++;
          }
        };
      };
      return (defect + arma::sum(parameter) + getNumberOfEvaluations());
    }
};

TEST_CASE("OptimationProblem.getLowerBounds(...)", "") {
  SECTION("Check default value") {
    std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(10));
    arma::Col<double>::fixed<10> expected;
    compare(optimisationProblem->getLowerBounds(), expected.fill(std::numeric_limits<double>::lowest()));
  }

  SECTION("Check parameterisation by .setLowerBounds(...)") {
    std::array<arma::Col<double>, 3> expecteds;
    expecteds.at(0) = arma::Col<double> {std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds.at(1) = arma::Col<double>({-56.89, 58.89});
    expecteds.at(2) = arma::Col<double>({1.0, 1.0, 1.0, 0.0, 0.0, 0.0});

    for (auto expected : expecteds) {
      std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(expected.n_elem));
      optimisationProblem->setLowerBounds(expected);
      compare(optimisationProblem->getLowerBounds(), expected);
    }
  }
}

TEST_CASE("OptimationProblem.getUpperBounds(...)", "") {
  SECTION("Check default value") {
    std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(10));
    arma::Col<double>::fixed<10> expected;
    compare(optimisationProblem->getUpperBounds(), expected.fill(std::numeric_limits<double>::max()));
  }

  SECTION("Check parametrisation of the upper bounds.") {
    std::array<arma::Col<double>, 3> expecteds;
    expecteds.at(0) = arma::Col<double> {std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds.at(1) = arma::Col<double>({-56.89, 58.89});
    expecteds.at(2) = arma::Col<double>({1.0, 1.0, 1.0, 0.0, 0.0, 0.0});

    for (auto& expected : expecteds) {
      std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(expected.n_elem));
      optimisationProblem->setUpperBounds(expected);
      compare(optimisationProblem->getUpperBounds(), expected);
    }
  }
}

TEST_CASE("OptimationProblem.setLowerBounds(...)", "") {
  SECTION("Check parametrisation of the lower bounds.") {
    std::array<arma::Col<double>, 3> expecteds;
    expecteds.at(0) = arma::Col<double> {std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds.at(1) = arma::Col<double>({-56.89, 58.89});
    expecteds.at(2) = arma::Col<double>({1.0, 1.0, 1.0, 0.0, 0.0, 0.0});

    for (auto& expected : expecteds) {
      std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(expected.n_elem));
      optimisationProblem->setLowerBounds(expected);
      compare(optimisationProblem->getLowerBounds(), expected);
    }
  }

  SECTION("Exception tests") {
    std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(2));

    SECTION("Throws an exception, if the number of elements of the lower bound is higher than the problem dimension") {
      arma::Col<double>::fixed<3> expected;
      CHECK_THROWS_AS(optimisationProblem->setLowerBounds(expected), std::logic_error);
    }

    SECTION("Throws an exception, if the number of elements of the lower bound is lower than the problem dimension") {
      arma::Col<double>::fixed<1> expected;
      CHECK_THROWS_AS(optimisationProblem->setLowerBounds(expected), std::logic_error);
    }
  }
}

TEST_CASE("OptimationProblem.setUpperBounds(...)", "") {
  SECTION("Check Sets the upper bounds of the search space of the upper bound.") {
    std::array<arma::Col<double>, 3> expecteds;
    expecteds.at(0) = arma::Col<double> {std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds.at(1) = arma::Col<double>({-56.89, 58.89});
    expecteds.at(2) = arma::Col<double>({1.0, 1.0, 1.0, 0.0, 0.0, 0.0});

    for (auto& expected : expecteds) {
      std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(expected.n_elem));
      optimisationProblem->setUpperBounds(expected);
      compare(optimisationProblem->getUpperBounds(), expected);
    }
  }

  SECTION("Exeption tests") {
    std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(2));

    SECTION("Throws an exception, if the number of elements of the upper bound is higher than the problem dimension") {
      arma::Col<double>::fixed<3> expected;
      CHECK_THROWS_AS(optimisationProblem->setUpperBounds(expected), std::logic_error);
    }

    SECTION("Throws an exception, if the number of elements of the upper bound is lower than the problem dimension") {
      arma::Col<double>::fixed<1> expected;
      CHECK_THROWS_AS(optimisationProblem->setUpperBounds(expected), std::logic_error);
    }
  }
}

TEST_CASE("OptimationProblem.getAcceptableObjectiveValue(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(10));

  SECTION("Check default value") {
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == std::numeric_limits<double>::lowest());
  }

  SECTION("Check parametrisation of the acceptable objective value") {
    optimisationProblem->setAcceptableObjectiveValue(std::numeric_limits<double>::min());
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == std::numeric_limits<double>::min());

    optimisationProblem->setAcceptableObjectiveValue(std::numeric_limits<double>::max());
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == std::numeric_limits<double>::max());

    optimisationProblem->setAcceptableObjectiveValue(0.0);
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == 0.0);

    optimisationProblem->setAcceptableObjectiveValue(-0.0);
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == -0.0);

    optimisationProblem->setAcceptableObjectiveValue(89.4569);
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == 89.4569);

    optimisationProblem->setAcceptableObjectiveValue(-183.09467);
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == -183.09467);
  }
}

TEST_CASE("OptimationProblem.setAcceptableObjectiveValue(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(10));

  SECTION("Check  .setAcceptableObjectiveValue(...)") {
    optimisationProblem->setAcceptableObjectiveValue(std::numeric_limits<double>::min());
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == std::numeric_limits<double>::min());

    optimisationProblem->setAcceptableObjectiveValue(std::numeric_limits<double>::max());
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == std::numeric_limits<double>::max());

    optimisationProblem->setAcceptableObjectiveValue(0.0);
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == 0.0);

    optimisationProblem->setAcceptableObjectiveValue(-0.0);
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == -0.0);

    optimisationProblem->setAcceptableObjectiveValue(89.4569);
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == 89.4569);

    optimisationProblem->setAcceptableObjectiveValue(-183.09467);
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == -183.09467);
  }
}

TEST_CASE("OptimationProblem.getCachedObjectiveValues(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(10));

  SECTION("Check returns the cached mapping of parameters to objective values.") {
    std::array<std::pair<arma::Col<double>, double>, 2> expecteds;
    expecteds.at(0).first = arma::Col<double> {std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds.at(1).first = arma::Col<double> {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    for (unsigned int z = 0; z < expecteds.size(); z++) {
      double defect = 0;
      for (unsigned int i = 0; i < expecteds.at(z).first.n_elem; i++) {
        for (unsigned int j = i; j < expecteds.at(z).first.n_elem; j++) {
          if (expecteds.at(z).first.at(i) > expecteds.at(z).first.at(j)) {
            defect++;
          }
        }
      }
      optimisationProblem->getObjectiveValue(expecteds.at(z).first);
      expecteds.at(z).second = defect + arma::sum(expecteds.at(z).first) + optimisationProblem->getNumberOfEvaluations();
    }

    std::unordered_map<arma::Col<double>, double, mant::Hash<double>, mant::IsEqual<double>> cache = optimisationProblem->getCachedObjectiveValues();


    for (auto value : cache) {
      bool isEqual = true;
      for (auto expected : expecteds) {
        for (unsigned int i = 0; i != expected.first.n_elem; i++) {
          if (expected.first.at(i) != value.first.at(i)) {
            isEqual = false;
            break;
          }
        }
        if (isEqual) {
          CHECK(value.second == expected.second);
        }
      }
    }
  }
}

TEST_CASE("OptimationProblem.getNumberOfDistinctEvaluations(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(3));

  SECTION("Check Counts the number of disctinct, i.e. unique objective function evaluations") {
    int count = 0;
    for (; count < 30; count++) {
      optimisationProblem->getObjectiveValue(arma::Col<double> {std::fmod(count * 3.78, 3), std::fmod(count * 3.78, 6), std::fmod(count * 3.78, 9)});
      optimisationProblem->getObjectiveValue(arma::Col<double> {std::fmod(count * 3.78, 3), std::fmod(count * 3.78, 6), std::fmod(count * 3.78, 9)});
    }
    CHECK(optimisationProblem->getNumberOfDistinctEvaluations() == count);
  }

  SECTION("Check use the .reset() method to reset this counter to 0.") {
    int count = 0;
    for (; count < 30; count++) {
      optimisationProblem->getObjectiveValue(arma::Col<double> {std::fmod(count * 3.78, 3), std::fmod(count * 3.78, 6), std::fmod(count * 3.78, 9)});
      optimisationProblem->getObjectiveValue(arma::Col<double> {std::fmod(count * 3.78, 3), std::fmod(count * 3.78, 6), std::fmod(count * 3.78, 9)});
    }
    optimisationProblem->reset();
    CHECK(optimisationProblem->getNumberOfDistinctEvaluations() == 0);
  }
}

TEST_CASE("OptimationProblem.getNumberOfEvaluations(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(3));

  SECTION("Check counts the number of ALL objective function evaluations") {
    int count = 0;
    for (; count < 30; count++) {
      optimisationProblem->getObjectiveValue(arma::Col<double> {std::fmod(count * 3.78, 3), std::fmod(count * 3.78, 6), std::fmod(count * 3.78, 9)});
    }
    CHECK(optimisationProblem->getNumberOfEvaluations() == count);
  }

  SECTION("Check use the .reset() method to reset this counter to 0.") {
    int count = 0;
    for (; count < 30; count++) {
      optimisationProblem->getObjectiveValue(arma::Col<double> {std::fmod(count * 3.78, 3), std::fmod(count * 3.78, 6), std::fmod(count * 3.78, 9)});
    }
    optimisationProblem->reset();
    CHECK(optimisationProblem->getNumberOfEvaluations() == 0);
  }

}

TEST_CASE("OptimationProblem.getObjectiveValue(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(10));

  SECTION("Check calculates the objective value.") {
    std::array<std::pair<arma::Col<double>, double>, 2> expecteds;
    expecteds.at(0).first = arma::Col<double> {std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds.at(1).first = arma::Col<double> {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    for (unsigned int z = 0; z < expecteds.size(); z++) {
      double defect = 0;
      for (unsigned int i = 0; i < expecteds.at(z).first.n_elem; i++) {
        for (unsigned int j = i; j < expecteds.at(z).first.n_elem; j++) {
          if (expecteds.at(z).first.at(i) > expecteds.at(z).first.at(j)) {
            defect++;
          }
        }
      }
      expecteds.at(z).second = defect + arma::sum(expecteds.at(z).first);
    }

    for (auto expected : expecteds) {
      double actual = optimisationProblem->getObjectiveValue(expected.first);
      CHECK(actual == expected.second + optimisationProblem->getNumberOfEvaluations());
    }
  }

  SECTION("Check retrieved from cache instead") {
    std::array<std::pair<arma::Col<double>, double>, 3> expecteds;
    expecteds[0].first = arma::Col<double> {std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds[1].first = arma::Col<double> {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    expecteds[2].first = expecteds[1].first;

    for (unsigned int z = 0; z < expecteds.size(); z++) {
      double defect = 0;
      for (unsigned int i = 0; i < expecteds.at(z).first.n_elem; i++) {
        for (unsigned int j = i; j < expecteds.at(z).first.n_elem; j++) {
          if (expecteds.at(z).first.at(i) > expecteds.at(z).first.at(j)) {
            defect++;
          }
        }
      }
      expecteds.at(z).second = defect + arma::sum(expecteds.at(z).first);
    }

    for (unsigned int i =0; i != expecteds.size(); i++) {
      double result = optimisationProblem->getObjectiveValue(expecteds.at(i).first);
      expecteds.at(i).second = expecteds.at(i).second + optimisationProblem->getNumberOfEvaluations();

      expecteds.at(2).second = expecteds.at(1).second;

      CHECK(result == expecteds.at(i).second);
    }
  }

  SECTION("Throws an exception, if the number of elements is higher than the problem dimension") {
    arma::Col<double> expected (12);
    CHECK_THROWS_AS(optimisationProblem->getObjectiveValue(expected), std::logic_error);
  }

  SECTION("Throws an exception, if the number of elements is lower than the problem dimension") {
    arma::Col<double> expected (9);
    CHECK_THROWS_AS(optimisationProblem->getObjectiveValue(expected), std::logic_error);
  }
}

TEST_CASE("OptimationProblem.getSoftConstraintsValue(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(10));

  SECTION("Check the default is to return 0 for all inputs.") {
    std::array<arma::Col<double>, 3> expected;
    expected.at(0) = arma::Col<double> {std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expected.at(1) = arma::Col<double> {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    expected.at(2) = arma::Col<double> {std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};

    for (auto value : expected) {
      CHECK(optimisationProblem->getSoftConstraintsValue(value) == 0);
    }
  }

  SECTION("Check calculates the soft constraint value") {
    std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestSoftConstraintsOptimisationProblem(10));
    std::array<std::pair<arma::Col<double>, double>, 3> expecteds;
    expecteds.at(0).first = arma::Col<double> {0.0, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds.at(1).first = arma::Col<double> {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    expecteds.at(2).first = arma::Col<double> {8578.458, -894.65, 458.7, -0.0, 0.0, 7529.25, -156.694456, 8.0, 96.96, -96.69};

    for (unsigned int z = 0; z < expecteds.size(); z++) {
      double defect = 0;
      for (unsigned int i = 0; i < expecteds.at(z).first.n_elem; i++) {
        for (unsigned int j = i; j < expecteds.at(z).first.n_elem; j++) {
          if (expecteds.at(z).first.at(i) > expecteds.at(z).first.at(j)) {
            defect++;
          }
        }
      }
      expecteds.at(z).second = defect + arma::sum(expecteds.at(z).first);
    }

    for (auto expected : expecteds) {
      CHECK((optimisationProblem->getSoftConstraintsValue(expected.first)) == expected.second);
    }
  }

  SECTION("Throws an exception, if the number of elements is higher than the problem dimension") {
    arma::Col<double> expected (12);
    CHECK_THROWS_AS(optimisationProblem->getSoftConstraintsValue(expected), std::logic_error);
  }

  SECTION("Throws an exception, if the number of elements is lower than the problem dimension") {
    arma::Col<double> expected (9);
    CHECK_THROWS_AS(optimisationProblem->getSoftConstraintsValue(expected), std::logic_error);
  }
}

TEST_CASE("OptimationProblem.isSatisfyingConstraints(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestSoftConstraintsOptimisationProblem(4));

  SECTION("Check returns true, if fulfills all bound- and soft-constraints and false otherwise.") {
    std::array<std::pair<arma::Col<double>, double>, 5> expecteds;
    expecteds.at(0).first = arma::Col<double> {134.0, 134.0, -60.96, -171.04};
    expecteds.at(1).first = arma::Col<double> {-345.0, -345.0, -345.0, -345.0};
    expecteds.at(2).first = arma::Col<double> {116.85, 130.89, -60.96, -186.78};
    expecteds.at(3).first = arma::Col<double> {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()};
    expecteds.at(4).first = arma::Col<double> {0.0, 0.0, 0.0, 0.0};

    for (unsigned int z = 0; z < expecteds.size(); z++) {
      double defect = 0;
      for (unsigned int i = 0; i < expecteds.at(z).first.n_elem; i++) {
        for (unsigned int j = i; j < expecteds.at(z).first.n_elem; j++) {
          if (expecteds.at(z).first.at(i) > expecteds.at(z).first.at(j)) {
            defect++;
          }
        }
      }
      expecteds.at(z).second = defect + arma::sum(expecteds.at(z).first);
    }

    optimisationProblem->setUpperBounds(expecteds.at(0).first);
    optimisationProblem->setLowerBounds(expecteds.at(1).first);

    for (auto expected : expecteds) {
      if (expected.second + optimisationProblem->getNumberOfEvaluations() == 0.0 && arma::all(expected.first <= expecteds.at(0).first) && arma::all(expected.first >= expecteds.at(1).first)) {
        CHECK(optimisationProblem->isSatisfyingConstraints(expected.first));
      }else{
        CHECK_FALSE(optimisationProblem->isSatisfyingConstraints(expected.first));
      }

    }
  }

  SECTION("Throws an exception, if the number of elements is higher than the problem dimension") {
    arma::Col<double> expected (9);
    CHECK_THROWS_AS(optimisationProblem->isSatisfyingConstraints(expected), std::logic_error);
  }

  SECTION("Throws an exception, if the number of elements is lower than the problem dimension") {
    arma::Col<double> expected (2);
    CHECK_THROWS_AS(optimisationProblem->isSatisfyingConstraints(expected), std::logic_error);
  }
}

TEST_CASE("OptimationProblem.isSatisfyingSoftConstraints(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestSoftConstraintsOptimisationProblem(4));

  SECTION("Check returns true, if the soft-constraints value is zero and false otherwise.") {
    std::array<std::pair<arma::Col<double>, double>, 3> expecteds;
    expecteds.at(0).first = arma::Col<double> {-56.89, 58.89, 56.89, -58.88};
    expecteds.at(1).first = arma::Col<double> {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()};
    expecteds.at(2).first = arma::Col<double> {0.0, 0.0, 0.0, 0.0};

    for (unsigned int z = 0; z < expecteds.size(); z++) {
      double defect = 0;
      for (unsigned int i = 0; i < expecteds.at(z).first.n_elem; i++) {
        for (unsigned int j = i; j < expecteds.at(z).first.n_elem; j++) {
          if (expecteds.at(z).first.at(i) > expecteds.at(z).first.at(j)) {
            defect++;
          }
        }
      }
      expecteds.at(z).second = defect + arma::sum(expecteds.at(z).first);
    }

    for (auto expected : expecteds) {
      if (expected.second + optimisationProblem->getNumberOfEvaluations() == 0) {
        CHECK(optimisationProblem->isSatisfyingSoftConstraints(expected.first));
      }else{
        REQUIRE_FALSE(optimisationProblem->isSatisfyingSoftConstraints(expected.first));
      }

    }
  }

  SECTION("Throws an exception, if the number of elements is higher than the problem dimension") {
    arma::Col<double> expected (9);
    CHECK_THROWS_AS(optimisationProblem->isSatisfyingSoftConstraints(expected), std::logic_error);
  }

  SECTION("Throws an exception, if the number of elements is lower than the problem dimension") {
    arma::Col<double> expected (2);
    CHECK_THROWS_AS(optimisationProblem->isSatisfyingSoftConstraints(expected), std::logic_error);
  }

}

TEST_CASE("OptimationProblem.isSatisfyingLowerBounds(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(4));

  SECTION("Check default value") {
    arma::Col<double>::fixed<4> expected;
    expected.fill(std::numeric_limits<double>::lowest());
    //CHECK(optimisationProblem->isSatisfyingLowerBounds(expected));
  }

  SECTION("Check returns true, if expected is element-wise equal to or greater then the lower bound and false otherwise.") {
    std::array<arma::Col<double>, 4> expecteds;
    expecteds.at(0) = arma::Col<double> {-56.89, 58.89, 8.98, 0.0};
    expecteds.at(1) = arma::Col<double> {std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min()};
    expecteds.at(2) = arma::Col<double> {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds.at(3) = arma::Col<double> {0.0, 0.0, 0.0, 0.0};

    optimisationProblem->setLowerBounds(expecteds.at(0));
    for (auto expected : expecteds) {
      if (arma::all(expected >= expecteds.at(0))) {
        //CHECK(optimisationProblem->isSatisfyingLowerBounds(expected));
      }else{
        //CHECK_FALSE(optimisationProblem->isSatisfyingLowerBounds(expected));
      }
    }
  }

  SECTION("Throws an exception, if the number of elements is higher than the problem dimension") {
    arma::Col<double> expected (9);
    //CHECK_THROWS_AS(optimisationProblem->isSatisfyingLowerBounds(expected), std::logic_error);
  }

  SECTION("Throws an exception, if the number of elements is lower than the problem dimension") {
    arma::Col<double> expected (2);
    //CHECK_THROWS_AS(optimisationProblem->isSatisfyingLowerBounds(expected), std::logic_error);
  }
}

TEST_CASE("OptimationProblem.isSatisfyingUpperBounds(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(4));

  SECTION("Check default value") {
    arma::Col<double>::fixed<4> expected;
    expected.fill(std::numeric_limits<double>::max());
    //CHECK(optimisationProblem->isSatisfyingUpperBounds(expected));
  }

  SECTION("Check returns true, if expected is element-wise equal to or less then the upper bound and false otherwise.") {
    std::array<arma::Col<double>, 4> expecteds;
    expecteds.at(0) = arma::Col<double> {-56.89, 58.89, 8.98, 0.0};
    expecteds.at(1) = arma::Col<double> {std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min()};
    expecteds.at(2) = arma::Col<double> {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    expecteds.at(3) = arma::Col<double> {0.0, 0.0, 0.0, 0.0};

    optimisationProblem->setUpperBounds(expecteds.at(0));
    for (auto expected : expecteds) {
      if (arma::all(expected <= expecteds.at(0))) {
        //CHECK(optimisationProblem->isSatisfyingUpperBounds(expected));
      }else{
        //CHECK_FALSE(optimisationProblem->isSatisfyingUpperBounds(expected));
      }
    }
  }

  SECTION("Throws an exception, if the number of elements is higher than the problem dimension") {
    arma::Col<double> expected (9);
    //CHECK_THROWS_AS(optimisationProblem->isSatisfyingUpperBounds(expected), std::logic_error);
  }

  SECTION("Throws an exception, if the number of elements is lower than the problem dimension") {
    arma::Col<double> expected (2);
    //CHECK_THROWS_AS(optimisationProblem->isSatisfyingUpperBounds(expected), std::logic_error);
  }
}

TEST_CASE("OptimationProblem.setObjectiveValueScaling(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(10));

  std::array<double, 4> scales{ {0.0, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), 23.8745} };

  std::array<std::pair<arma::Col<double>, double>, 2> expecteds;
  expecteds.at(0).first = arma::Col<double> {std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
  expecteds.at(1).first = arma::Col<double> {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  for (unsigned int z = 0; z < expecteds.size(); z++) {
    double defect = 0;
    for (unsigned int i = 0; i < expecteds.at(z).first.n_elem; i++) {
      for (unsigned int j = i; j < expecteds.at(z).first.n_elem; j++) {
        if (expecteds.at(z).first.at(i) > expecteds.at(z).first.at(j)) {
          defect++;
        }
      }
    }
    expecteds.at(z).second = defect + arma::sum(expecteds.at(z).first);
  }

  SECTION("Parameterises the scaling of the objective value.") {
    for (auto scale : scales) {
      optimisationProblem->setObjectiveValueScaling(scale);
      for (auto expected : expecteds) {
        //CHECK((optimisationProblem->getObjectiveValue(expected.first)) == (expected.second + optimisationProblem->getNumberOfEvaluations() * scale));
      }
    }
  }
}

TEST_CASE("OptimationProblem.setObjectiveValueTranslation(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(10));

  std::array<double, 4> translations{ {0.0, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), 23.8745} };

  std::array<std::pair<arma::Col<double>, double>, 2> expecteds;
  expecteds.at(0).first = arma::Col<double> {std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
  expecteds.at(1).first = arma::Col<double> {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  for (unsigned int z = 0; z < expecteds.size(); z++) {
    double defect = 0;
    for (unsigned int i = 0; i < expecteds.at(z).first.n_elem; i++) {
      for (unsigned int j = i; j < expecteds.at(z).first.n_elem; j++) {
        if (expecteds.at(z).first.at(i) > expecteds.at(z).first.at(j)) {
          defect++;
        }
      }
    }
    expecteds[z].second = defect + arma::sum(expecteds.at(z).first);
  }

  SECTION("Parameterises the translation of the objective value.") {
    for (auto translation : translations) {
      optimisationProblem->setObjectiveValueTranslation(translation);
      for (auto expected : expecteds) {
        //CHECK((optimisationProblem->getObjectiveValue(expected.first)) == (expected.second + optimisationProblem->getNumberOfEvaluations() + translation));
      }
    }
  }
}

TEST_CASE("OptimationProblem.setParameterPermutation(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(5));

  SECTION("Parameterises the permutation of the dimensions of the objective function.") {
    std::array<arma::Col<unsigned int>, 5> permutations = {
      arma::Col<unsigned int>{0, 1, 2, 3, 4},
      arma::Col<unsigned int>{0, 2, 1, 3, 4},
      arma::Col<unsigned int>{0, 1, 3, 2, 4},
      arma::Col<unsigned int>{4, 1, 2, 3, 0},
      arma::Col<unsigned int>{0, 3, 2, 1, 4}
    };

    arma::Col<double>::fixed<5> vector = {1.0, 2.0, 3.0, 4.0, 5.0};

    for (auto permutation : permutations) {
      arma::Col<double>::fixed<5> expected;
      double defect = 0;

      for (unsigned int i=0; i != permutation.n_elem;i++) {
        expected.at(permutation.at(i)) = vector.at(i);
      }

      for (unsigned int i = 0; i < expected.n_elem; i++) {
        for (unsigned int j = i; j < expected.n_elem; j++) {
          if (expected.at(i) > expected.at(j)) {
            defect++;
          }
        }
      }
      optimisationProblem->setParameterPermutation(permutation);
      //CHECK (optimisationProblem->getObjectiveValue(vector) == (defect + arma::sum(expected) + optimisationProblem->getNumberOfEvaluations()));
    }
  }

  SECTION("Throws an exception, the number of elements < problem dimension") {
    arma::Col<unsigned int> expected (3);
    CHECK_THROWS_AS(optimisationProblem->setParameterPermutation(expected), std::logic_error);
  }

  SECTION("Throws an exception, the number of elements > problem dimension") {
    arma::Col<unsigned int> expected (6);
    CHECK_THROWS_AS(optimisationProblem->setParameterPermutation(expected), std::logic_error);
  }

  SECTION("Throws an exception, all elements must be unique and within [0, number of dimensions - 1]") {
    arma::Col<unsigned int> permutations [] = {
      arma::Col<unsigned int>{0, 5, 2, 3, 4},
      arma::Col<unsigned int>{0, 1, 1, 3, 6}};

    for (auto value:permutations) {
      // CHECK_THROWS_AS(optimisationProblem->setParameterPermutation(value), std::logic_error);
    }
  }
}

TEST_CASE("OptimationProblem.setParameterRotation(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(5));

  SECTION(" Parameterises the rotation of the objective function.") {
    arma::Mat<double>::fixed<5, 5> rotation = {
      1.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 1.0
    };

    arma::Col<double> parameter = {0.0, -96.96, 87.56, 5.2, 326.4};

    double defect = 0;
    for (unsigned int i = 0 ; i < parameter.n_elem; i++) {
      for (unsigned int j = i; j < parameter.n_elem; j++) {
        if (parameter.at(i) > parameter.at(j)) {
          defect++;
        }
      }
    }

    optimisationProblem->setParameterRotation(rotation);
    double actual = optimisationProblem->getObjectiveValue(parameter);
    CHECK (actual == (defect + arma::sum(rotation * parameter) + optimisationProblem->getNumberOfEvaluations()));

  }

  SECTION("Throws an exception, must be square") {
    arma::Mat<double> expected (3, 7);
    CHECK_THROWS_AS(optimisationProblem->setParameterRotation(expected), std::logic_error);
  }

  SECTION("Throws an exception, must be orthonormal") {
    arma::Mat<double> expected = {
      0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0
    };
    
    CHECK_THROWS_AS(optimisationProblem->setParameterRotation(expected), std::logic_error);
  }

  SECTION("Throws an exception, the number of elements < problem dimension") {
    arma::Mat<double> expected (3, 3);
    CHECK_THROWS_AS(optimisationProblem->setParameterRotation(expected), std::logic_error);
  }

  SECTION("Throws an exception, the number of elements > problem dimension") {
    arma::Mat<double> expected (6, 6);
    CHECK_THROWS_AS(optimisationProblem->setParameterRotation(expected), std::logic_error);
  }
}

TEST_CASE("OptimationProblem.setParameterScaling(...)", "") {

  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(5));

  SECTION("Parameterises the scaling of the objective function.") {
    std::array<arma::Col<double>, 3> scalings;
    scalings.at(0) = arma::Col<double>{8.56, -9.56, 0.0, 9.2, 77.1};
    scalings.at(1) = arma::Col<double>{7.8, 2.1, -7.2, 77.77, 8.2};
    scalings.at(2) = arma::Col<double>{1.0, 1.0, 1.0, 1.0, 1.0};

    arma::Col<double> parameter = {0.0, -96.96, 87.56, 5.2, 326.4};

    for (auto scaling : scalings) {
      arma::Col<double> expected = parameter%scaling;

      double defect = 0;
      for (unsigned int i = 0 ; i < expected.n_elem; i++) {
        for (unsigned int j = i; j < expected.n_elem; j++) {
          if (expected.at(i) > expected.at(j)) {
            defect++;
          }
        }
      }

      optimisationProblem->setParameterScaling(scaling);

      //CHECK (optimisationProblem->getObjectiveValue(parameter) == (defect + arma::sum(expected) + optimisationProblem->getNumberOfEvaluations()));
    }
  }

  SECTION("Throws an exception, the number of elements < problem dimension") {
    arma::Col<double> expected (3);
    CHECK_THROWS_AS(optimisationProblem->setParameterScaling(expected), std::logic_error);
  }

  SECTION("Throws an exception, the number of elements > problem dimension") {
    arma::Col<double> expected (6);
    CHECK_THROWS_AS(optimisationProblem->setParameterScaling(expected), std::logic_error);
  }
}

TEST_CASE("OptimationProblem.setParameterTranslation(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(5));

  SECTION("Parameterises the translation of the objective function.") {
    std::array<arma::Col<double>, 3> translations;
    translations.at(0) = arma::Col<double>{8.56, -9.56, 0.0, 9.2, 77.1};
    translations.at(1) = arma::Col<double>{7.8, 2.1, -7.2, 77.77, 8.2};
    translations.at(2) = arma::Col<double>{1.0, 1.0, 1.0, 1.0, 1.0};

    arma::Col<double> parameter = {0.0, -96.96, 87.56, 5.2, 326.4};

    for (auto translation : translations) {
      arma::Col<double> expected = parameter + translation;

      double defect = 0;
      for (unsigned int i = 0 ; i < expected.n_elem; i++) {
        for (unsigned int j = i; j < expected.n_elem; j++) {
          if (expected.at(i) > expected.at(j)) {
            defect++;
          }
        }
      }

      optimisationProblem->setParameterTranslation(translation);

      //CHECK (optimisationProblem->getObjectiveValue(parameter) == (defect + arma::sum(expected) + optimisationProblem->getNumberOfEvaluations()));
    }
  }

  SECTION("Throws an exception, the number of elements < problem dimension") {
    arma::Col<double> expected (3);
    CHECK_THROWS_AS(optimisationProblem->setParameterTranslation(expected), std::logic_error);
  }

  SECTION("Throws an exception, the number of elements > problem dimension") {
    arma::Col<double> expected (6);
    CHECK_THROWS_AS(optimisationProblem->setParameterTranslation(expected), std::logic_error);
  }
}

TEST_CASE("OptimationProblem.reset(...)", "") {
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new TestOptimisationProblem(4));

  optimisationProblem->setUpperBounds(arma::Col<double> {-56.89, 58.89, 88.98, 78.0});
  optimisationProblem->setLowerBounds(arma::Col<double> {-56.89, -58.89, -88.98, -78.0});
  optimisationProblem->setAcceptableObjectiveValue(45.458);

  for (unsigned int i = 0; i != 23; i++) {
    optimisationProblem->getObjectiveValue(arma::Col<double>{1.0, 1.0, 1.0, 1.0} * i);
  }
  optimisationProblem->getObjectiveValue(arma::Col<double>{1.0, 1.0, 1.0, 1.0});
  optimisationProblem->getObjectiveValue(arma::Col<double>{22.0, 22.0, 22.0, 22.0});


  SECTION("Check currect values") {
    CHECK(optimisationProblem->getNumberOfDistinctEvaluations() == 23);
    CHECK(optimisationProblem->getNumberOfEvaluations() == 25);
    CHECK(!optimisationProblem->getCachedObjectiveValues().empty());
  }

  SECTION("Check number of evaluations") {
    optimisationProblem->reset();
    CHECK(optimisationProblem->getNumberOfEvaluations() == 0);
  }

  SECTION("Check number of distinct evaluations") {
    optimisationProblem->reset();
    CHECK(optimisationProblem->getNumberOfDistinctEvaluations() == 0);
  }

  SECTION("Check cached objective values") {
    optimisationProblem->reset();
    CHECK(optimisationProblem->getCachedObjectiveValues().empty());
  }

  SECTION("Check upper bounds") {
    optimisationProblem->reset();
    compare(optimisationProblem->getUpperBounds(), arma::Col<double>{-56.89, 58.89, 88.98, 78.0});
  }

  SECTION("Check lower bounds") {
    optimisationProblem->reset();
    compare(optimisationProblem->getLowerBounds(), arma::Col<double>{-56.89, -58.89, -88.98, -78.0});
  }

  SECTION("Check acceptable objective value") {
    optimisationProblem->reset();
    CHECK(optimisationProblem->getAcceptableObjectiveValue() == 45.458);
  }

  SECTION("Check objective value scaling") {
    optimisationProblem->setObjectiveValueScaling(0.58);
    optimisationProblem->reset();
    CHECK(optimisationProblem->getObjectiveValue(arma::Col<double>{1.0, 1.0, 1.0, 1.0}) == 5.0 * 0.58);
  }

  SECTION("Check objective value translation") {
    optimisationProblem->setObjectiveValueTranslation(15);
    optimisationProblem->reset();
    CHECK(optimisationProblem->getObjectiveValue(arma::Col<double>{1.0, 1.0, 1.0, 1.0}) == 5.0 + 15.0);
  }

  SECTION("Check parameter permutation") {
    optimisationProblem->setParameterPermutation(arma::Col<unsigned int> {0, 1, 3, 2});
    optimisationProblem->reset();
    CHECK(optimisationProblem->getObjectiveValue(arma::Col<double>{1.0, 1.0, 1.0, 2.0}) == 7.0);
  }

  SECTION("Check parameter rotation") {
    arma::Mat<double>::fixed<4, 4> rotation = {
      1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0
    };

    optimisationProblem->setParameterRotation(rotation);
    optimisationProblem->reset();
    CHECK(optimisationProblem->getObjectiveValue(arma::Col<double>{1.0, 1.0, 1.0, 1.0}) == 5.0);
  }

  SECTION("Check parameter scaling") {
    optimisationProblem->setParameterScaling(arma::Col<double> {0.459, -7.25, 6.0, 17.08});
    optimisationProblem->reset();
    arma::Col<double> expected = arma::Col<double>{1.0, 1.0, 1.0, 1.0} % arma::Col<double>{0.459, -7.25, 6.0, 17.08};

    double defect = 0;
    for (unsigned int i = 0 ; i < expected.n_elem; i++) {
      for (unsigned int j = i; j < expected.n_elem; j++) {
        if (expected.at(i) > expected.at(j))  {
          defect++;
        }
      }
    }
    double actual = optimisationProblem->getObjectiveValue(arma::Col<double>{1.0, 1.0, 1.0, 1.0});
    CHECK(actual == arma::sum(expected) + defect + optimisationProblem->getNumberOfEvaluations());
  }

  SECTION("Check parameter translation") {
    optimisationProblem->setParameterTranslation(arma::Col<double> {5.4569, 4.2, 5.0, -0.0003});
    optimisationProblem->reset();
    arma::Col<double> expected = arma::Col<double>{1.0, 1.0, 1.0, 1.0} - arma::Col<double>{5.4569, 4.2, 5.0, -0.0003};

    double defect = 0;
    for (unsigned int i = 0 ; i < expected.n_elem; i++) {
      for (unsigned int j = i; j < expected.n_elem; j++) {
        if (expected.at(i) > expected.at(j))  {
          defect++;
        }
      }
    }
    double actual = optimisationProblem->getObjectiveValue(arma::Col<double>{1.0, 1.0, 1.0, 1.0});
    CHECK(actual == arma::sum(expected) + defect + optimisationProblem->getNumberOfEvaluations());
  }
}
