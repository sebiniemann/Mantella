// Catch
#include <catch.hpp>

// C++ standard library
#include <random>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE(
    "PassivePropertyAnalysis") {
  class DummyPassivePropertyAnalysis : public mant::PassivePropertyAnalysis {
   public:
    using mant::PassivePropertyAnalysis::PassivePropertyAnalysis;

    void analyseImplementation() override {
      propertyValue_ = 5.0;
    }

    double getPropertyValue() {
      return propertyValue_;
    }

    std::string toString() const override {
      return "test_passive_property_analysis";
    }

   protected:
    double propertyValue_;
  };

  SECTION(
      "Constructor") {
    SECTION(
        "Exception tests") {
      SECTION(
          "Throw an exception, if the number of samples less than 2.") {
        std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples = {
            {arma::randu<arma::Col<double>>(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())) * 200 - 100, std::uniform_real_distribution<double>(-10.0, 10.0)(mant::Rng::getGenerator())}};
        CHECK_THROWS_AS(DummyPassivePropertyAnalysis passivePropertyAnalysis(samples), std::logic_error);
      }

      SECTION(
          "Throw an exception, if the any two samples have a different number of dimensions.") {
        const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
        std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples = {
            {arma::randu<arma::Col<double>>(numberOfDimensions) * 200 - 100, std::uniform_real_distribution<double>(-10.0, 10.0)(mant::Rng::getGenerator())},
            {arma::randu<arma::Col<double>>(numberOfDimensions + 1) * 200 - 100, std::uniform_real_distribution<double>(-10.0, 10.0)(mant::Rng::getGenerator())},
        };

        CHECK_THROWS_AS(DummyPassivePropertyAnalysis passivePropertyAnalysis(samples), std::logic_error);
      }
    }
  }

  SECTION(
      ".analyse") {
    SECTION(
        "Checking the property value.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();

      DummyPassivePropertyAnalysis passivePropertyAnalysis(optimisationProblem->getCachedSamples());
      passivePropertyAnalysis.analyse();

      CHECK(passivePropertyAnalysis.getPropertyValue() == 5.0);
    }
  }
}
