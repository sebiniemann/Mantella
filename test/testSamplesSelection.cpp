// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("elitists", "[samplesSelection][elitists]") {
  GIVEN("A set of samples and a number of samples to select") {
    WHEN("The set is empty") {
      THEN("Return an empty set") {
        CHECK(mant::elitists(std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({}), 0).empty() == true);
      }
    }

    WHEN("The number of samples to select is 0") {
      THEN("Return an empty set") {
        CHECK(mant::elitists(std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}}), 0).empty() == true);
      }
    }

    WHEN("The number of samples to select is equal to the set's size") {
      THEN("Return the set") {
        const std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>& expectedSet = {{{0.0, -3.0}, -1.0}, {{2.0, -3.0}, 4.0}};
        CHECK(mant::elitists(std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0, -3.0}, -1.0}, {{2.0, -3.0}, 4.0}}), 2) == expectedSet);
      }
    }

    WHEN("The number of samples to select is less than the set's size") {
      THEN("Return the elitists") {
        const std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>& expectedSet = {{{0.0, -3.0}, -1.0}, {{2.0, -1.0}, -4.0}};
        CHECK(mant::elitists(std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{2.0, -3.0}, 3.0}, {{0.0, -3.0}, -1.0}, {{2.0, -1.0}, -4.0}}), 2) == expectedSet);
      }
    }
  }
}
