// Armadillo
#include <armadillo>

// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("gravityAssist", "[orbitalMechanics][gravityAssist]") {
  GIVEN("An inbound and outbound velocity vector") {
    WHEN("Two different vectors are given") {
      THEN("Return std::pair with velocity difference and rp") {
        arma::Col<double>::fixed<3> inboundVector = {123.0, 246.0, 369.0};
        arma::Col<double>::fixed<3> outboundVector = {234.0, 135.0, 470.0};
        std::pair<double, double> resultPair = mant::itd::gravityAssist(inboundVector, outboundVector);
        CHECK(resultPair.first == Approx(69.2261));
        CHECK(resultPair.second == Approx(2.0046e-05));
      }
    }

    WHEN("Inbound and outbound vector are equal") {
      THEN("Throw a std::logic_error") {
        arma::Col<double>::fixed<3> inboundVector = {123.0, 246.0, 369.0};
        arma::Col<double>::fixed<3> outboundVector = {123.0, 246.0, 369.0};

        CHECK_THROWS_AS(mant::itd::gravityAssist(inboundVector, outboundVector), std::logic_error);
      }
    }

    WHEN("Inbound and outbound vector have infinty values") {
      THEN("Throw a std::logic_error") {
        arma::Col<double>::fixed<3> inboundVector = {arma::datum::inf, arma::datum::inf, arma::datum::inf};
        arma::Col<double>::fixed<3> outboundVector = {arma::datum::inf, arma::datum::inf, arma::datum::inf};

        CHECK_THROWS_AS(mant::itd::gravityAssist(inboundVector, outboundVector), std::logic_error);
      }
    }
  }
}
