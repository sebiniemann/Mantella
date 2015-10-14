// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <cmath>

// Mantella
#include <mantella>

TEST_CASE("getMedianAbsoluteError") {
  SECTION("Returns the median absolute error.") {
    const arma::Row<double>& randomValues = getRandomValues(1001);
    CAPTURE(arma::sort(randomValues, "descend"));
  
    CHECK(mant::getMedianAbsoluteError(randomValues) == Approx(mant::getMedian(arma::abs(randomValues - static_cast<arma::Row<double>>(arma::sort(randomValues))(500)))));
  }
}

TEST_CASE("getPercentile") {
  const arma::Row<double>& randomValues = getRandomValues(1000);
  CAPTURE(arma::sort(randomValues, "descend"));
    
  SECTION("Returns the percentile.") {
    CHECK(mant::getPercentile(randomValues, 1.0) == Approx(static_cast<arma::Row<double>>(arma::sort(randomValues, "descend"))(10)));
  }

  SECTION("Exception tests") {
    SECTION("Throws an exception, if the percentile is lesser than or equal to 0.") {
      CHECK_THROWS_AS(mant::getPercentile(randomValues, 0), std::logic_error);
    }
    
    SECTION("Throws an exception, if the percentile is greater than 100.") {
      CHECK_THROWS_AS(mant::getPercentile(randomValues, 100.1), std::logic_error);
    }
  }
}

TEST_CASE("getDecile") {
  const arma::Row<double>& randomValues = getRandomValues(1000);
  CAPTURE(arma::sort(randomValues, "descend"));
    
  SECTION("Returns the decile.") {
    CHECK(mant::getDecile(randomValues, 9.0) == Approx(static_cast<arma::Row<double>>(arma::sort(randomValues, "descend"))(900)));
  }

  SECTION("Exception tests") {
    SECTION("Throws an exception, if the percentile is lesser than or equal to 0.") {
      CHECK_THROWS_AS(mant::getDecile(randomValues, 0), std::logic_error);
    }
    
    SECTION("Throws an exception, if the percentile is greater than 10.") {
      CHECK_THROWS_AS(mant::getDecile(randomValues, 10.1), std::logic_error);
    }
  }
}

TEST_CASE("getQuartile") {
    const arma::Row<double>& randomValues = getRandomValues(1000);
    CAPTURE(arma::sort(randomValues, "descend"));
  SECTION("Returns the quartile.") {
  
    CHECK(mant::getQuartile(randomValues, 3.0) == Approx(static_cast<arma::Row<double>>(arma::sort(randomValues, "descend"))(750)));
  }
  
  SECTION("Exception tests") {
    SECTION("Throws an exception, if the percentile is lesser than or equal to 0.") {
      CHECK_THROWS_AS(mant::getQuartile(randomValues, 0), std::logic_error);
    }
    
    SECTION("Throws an exception, if the percentile is greater than 4.") {
      CHECK_THROWS_AS(mant::getQuartile(randomValues, 4.1), std::logic_error);
    }
  }
}

TEST_CASE("getMedian") {
  SECTION("Returns the median.") {
    const arma::Row<double>& randomValues = getRandomValues(1001);
    CAPTURE(arma::sort(randomValues, "descend"));
  
    CHECK(mant::getMedian(randomValues) == Approx(static_cast<arma::Row<double>>(arma::sort(randomValues, "descend"))(500)));
  }
}
