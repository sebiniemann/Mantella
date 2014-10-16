#include <catch/catch.hpp>

#include <cstdlib>

#include <armadillo>

#include <hop>

extern std::string testDirectory;

TEST_CASE("Geometry helper", "[geometry]") {

  SECTION("Test impementation correctness") {
    arma::Mat<double>::fixed<2, 2> firstCenter({
      -2, 3,
      1.8, -2.5
    });
    arma::Row<double>::fixed<2> firstRadius({
      1.3,
      4
    });

    arma::Mat<double>::fixed<2, 2> secondCenter({
      -1.5, 2.4,
      -3, 2
    });
    arma::Row<double>::fixed<2> secondRadius({
      1.2,
      3
    });

    arma::Mat<double>::fixed<2, 2> expected({
      -2.5522451636, 1.8231290303,
      -0.1804143359, 0.9753358195
    });

    arma::Mat<double> result = hop::Geometry::getCircleCircleIntersection(firstCenter, firstRadius, secondCenter, secondRadius);
    for (std::size_t n = 0; n < expected.n_elem; ++n) {
      CHECK(result.at(n) == Approx(expected.at(n)));
    }
  }
}

