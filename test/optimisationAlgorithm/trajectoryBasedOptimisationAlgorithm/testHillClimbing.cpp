// Catch
#include <catch.hpp>

// C++ Standard Library
#include <memory>

// Mantella
#include <mantella>

TEST_CASE("HillClimbing", "") {
  SECTION(".setMaximalStepSize") {
    SECTION("Test default value"){
      //TODO
    }

    SECTION("Test with parameter") {
      //TODO
    }
  }

  SECTION(".optimise") {
    // TODO
  }

  SECTION("Exception tests") {
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    mant::HillClimbing hillClimbing(optimisationProblem);

    SECTION("Throws an exception, if the MaximalStepSize zero") {
      CHECK_THROWS_AS(hillClimbing.setMaximalStepSize({0, 0}), std::logic_error);
    }

    SECTION("Throws an exception, if the size of MaximalStepSize is not equal to the number of dimension of the problem") {
      CHECK_THROWS_AS(hillClimbing.setMaximalStepSize({100, 100, 100}), std::logic_error);
      CHECK_THROWS_AS(hillClimbing.setMaximalStepSize({100}), std::logic_error);
    }

  }

  SECTION("Returns the specified class name.") {
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    CHECK(mant::HillClimbing(optimisationProblem).toString() == "hill_climbing");
  }
}
