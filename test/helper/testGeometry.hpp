#include <catch/catch.hpp>

#include <cstdlib>
#include <cmath>
#include <array>

#include <armadillo>

#include <hop>

extern std::string testDirectory;

TEST_CASE("Geometry helper", "[geometry]") {

  SECTION("Test impementation correctness") {
    arma::Mat<double>::fixed<2, 2> firstCenter({
      -2.0, 3.0,
      1.8, -2.5
    });
    arma::Row<double>::fixed<2> firstRadius({
      1.3,
      4.0
    });

    arma::Mat<double>::fixed<2, 2> secondCenter({
      -1.5, 2.4,
      -3.0, 2.0
    });
    arma::Row<double>::fixed<2> secondRadius({
      1.2,
      3.0
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

  SECTION("Test rotation matrix impementation 2D") {

    std::array<double, 15> angles = {0.0,  45.0, 90.0, 135.0, 180.0, 225.0, 270.0, 315.0, 360.0, -0.0, -45.0, -90.0, -180.0, -225.0, -315.0};

    for (auto angle : angles) {
      arma::Mat<double> result = hop::Geometry::get2DRotationMatrix(angle);

      arma::Mat<double>::fixed<2, 2> expected({
        std::cos(angle), -std::sin(angle),
        std::sin(angle), std::cos(angle)
      });

      //CHECK(result.n_rows == expected.n_rows);
      //CHECK(result.n_cols == expected.n_cols);

      for (std::size_t nn = 0; nn < expected.n_elem; ++nn) {
        CHECK(result.at(nn) == Approx(expected.at(nn)));
      }
    }
  }

  SECTION("Test rotation matrix impementation 3D") {

    double rollAngle[] = {0,45,90,135,180,225,270,315,360,-0,-45,276,-56,-45.89};
    double pitchAngle[] = {0,45,90,135,180,225,270,315,360,-0,-90,-89,78,-245};
    double yawAngle[] = {0,45,90,135,180,225,270,315,360,-0,-225,-310,-90,345};

    for (std::size_t n = 0; n != sizeof(rollAngle); ++n){
        arma::Mat<double> result = hop::Geometry::get3DRotationMatrix(rollAngle[n],pitchAngle[n],yawAngle[n]);

        arma::Mat<double>::fixed<3, 3> expectedRoll({
          1, 0, 0,
          0, std::cos(rollAngle[n]), -std::sin(rollAngle[n]),
          0, std::sin(rollAngle[n]), std::cos(rollAngle[n]),
        });

        arma::Mat<double>::fixed<3, 3> expectedPitch({
          std::cos(pitchAngle[n]), 0, std::sin(pitchAngle[n]),
          0, 1, 0,
          -std::sin(pitchAngle[n]), 0, std::cos(pitchAngle[n])
        });

        arma::Mat<double>::fixed<3, 3> expectedYaw({
          std::cos(yawAngle[n]), -std::sin(yawAngle[n]), 0,
          std::sin(yawAngle[n]), std::cos(yawAngle[n]), 0,
          0, 0, 1
        });
        //arma::Mat<double>::fixed<3, 3> expected = expectedYaw*expectedPitch*expectedRoll;
        arma::Mat<double>::fixed<3, 3> expected = expectedRoll*expectedPitch*expectedYaw;

        for (std::size_t i = 0; i < expected.n_elem; ++i) {
          CHECK(result.at(i) == Approx(expected.at(i)));
        }

    }
  }
}

