// Catch
#include <catch.hpp>

// C++ Standard Library
#include <memory>

// Mantella
#include <mantella>

TEST_CASE(
    "HookeJeevesAlgorithm") {
  SECTION(
      ".optimise") {
    // TODO
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns the expected class name.") {
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
      CHECK(mant::HookeJeevesAlgorithm(optimisationProblem).toString() ==
            "hooke_jeeves_algorithm");
    }
  }
}
