// Catch
#include <catch.hpp>

// Armadillo
#include <armadillo>

// HOP
#include <hop>

TEST_CASE("Hill climbing", "") {
  CHECK(hop::endsWith("input.serialised", ".serialised") == true);
  CHECK(hop::endsWith("input.serialised", "serialised") == true);
  CHECK(hop::endsWith("input.serialised", ".serialise") == false);
  CHECK(hop::endsWith("input.serialised", ".mat") == false);
}

class TestHillClimbing : public hop::HillClimbing {
  public:
    arma::Col<double> getVelocity() override {
        return nullptr; // TODO make it work
    }
};

class TestHillClimbingProblem : public hop::OptimisationProblem {
  protected:
    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override {

    }
};
