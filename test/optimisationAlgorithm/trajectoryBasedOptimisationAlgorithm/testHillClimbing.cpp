// Catch
#include <catch.hpp>

// C++ Standard Library
#include <memory>
#include <random>

// Mantella
#include <mantella>

class TestHillClimbing : public mant::HillClimbing {
 public:
  TestHillClimbing(
      const std::shared_ptr<mant::OptimisationProblem> optimisationProblem)
      : mant::HillClimbing(optimisationProblem),
        neighboursIndex_(0) {
  }

  void setVelocitys(
      const arma::Mat<double>& neighbours) {
    neighbours_ = neighbours;
  }

 protected:
  arma::Col<double> getRandomNeighbour(
      const arma::Col<double>& parameter,
      const arma::Col<double>& minimalDistance,
      const arma::Col<double>& maximalDistance) override {
    return neighbours_.col(neighboursIndex_++);
  }

  arma::uword neighboursIndex_;
  arma::Mat<double> neighbours_;
};

TEST_CASE(
    "HillClimbing") {
  SECTION(
      ".setMaximalStepSize") {
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    mant::HillClimbing hillClimbing(optimisationProblem);

    SECTION(
        "Test default value") {
      //TODO
    }

    SECTION(
        "Test with parameter") {
      //TODO
    }
  }

  SECTION(
      ".optimise") {
    // TODO
  }

  SECTION(
      "Exception tests") {
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    mant::HillClimbing hillClimbing(optimisationProblem);

    SECTION(
        "Throws an exception, if the MaximalStepSize zero") {
      CHECK_THROWS_AS(hillClimbing.setMaximalStepSize({0, 0}), std::logic_error);
    }

    SECTION(
        "Throws an exception, if the size of MaximalStepSize is not equal to the number of dimension of the problem") {
      CHECK_THROWS_AS(hillClimbing.setMaximalStepSize(arma::randu<arma::Mat<double>>(std::uniform_int_distribution<arma::uword>(3, 10)(mant::Rng::getGenerator())) * 200 - 100), std::logic_error);
      CHECK_THROWS_AS(hillClimbing.setMaximalStepSize(arma::randu<arma::Mat<double>>(1) * 200 - 100), std::logic_error);
    }
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns the expected class name.") {
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
      CHECK(mant::HillClimbing(optimisationProblem).toString() ==
            "hill_climbing");
    }
  }
}
