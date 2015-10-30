// Catch
#include <catch.hpp>

// Mantella
#include <mantella>

TEST_CASE(
    "ActivePropertyAnalysis") {
  class DummyActivePropertyAnalysis : public mant::ActivePropertyAnalysis {
   public:
    using mant::ActivePropertyAnalysis::ActivePropertyAnalysis;

    void analyseImplementation() override {
      propertyValue_ = 5.0;
    }

    double getPropertyValue() {
      return propertyValue_;
    }

    std::string toString() const override {
      return "test_active_property_analysis";
    }

   protected:
    double propertyValue_;
  };

  SECTION(
      ".analyse") {
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    DummyActivePropertyAnalysis activePropertyAnalysis(optimisationProblem);

    SECTION(
        "Checking the property value.") {
      activePropertyAnalysis.analyse();

      CHECK(activePropertyAnalysis.getPropertyValue() == 5.0);
    }

    SECTION(
        "Exception tests") {
      SECTION(
          "Throw an exception, if for any dimension the lower bound is greater than the upper bound.") {
        optimisationProblem->setUpperBounds(arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 200 - 100);
        optimisationProblem->setLowerBounds(optimisationProblem->getUpperBounds() + arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 100 + 0.01);

        CHECK_THROWS_AS(activePropertyAnalysis.analyse(), std::logic_error);
      }
    }
  }
}
