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

  SECTION("Test rotation matrix impementation 2D") {

    double angle[] = {0,45,90,135,180,225,270,315,360,-0,-45,-90,-180,-225,-315};

    for (std::size_t n = 0; n != sizeof(angle); ++n){
        arma::Mat<double> result = hop::Geometry::get2DRotationMatrix(angle[n]);

        arma::Mat<double>::fixed<2, 2> expected({
          cos(angle[n]), -sin(angle[n]),
          sin(angle[n]), cos(angle[n])
        });

        for (std::size_t i = 0; i < expected.n_elem; ++i) {
          CHECK(result.at(i) == expected.at(i));
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
          1,0,0,
          0,cos(rollAngle[n]), -sin(rollAngle[n]),
          0,sin(rollAngle[n]), cos(rollAngle[n]),
        });

        arma::Mat<double>::fixed<3, 3> expectedPitch({
          cos(pitchAngle[n]),0, sin(pitchAngle[n]),
          0,1,0,
          -sin(pitchAngle[n]),0, cos(pitchAngle[n])
        });

        arma::Mat<double>::fixed<3, 3> expectedYaw({
          cos(yawAngle[n]), -sin(yawAngle[n]),0,
          sin(yawAngle[n]), cos(yawAngle[n]),0,
          0,0,1
        });
        //arma::Mat<double>::fixed<3, 3> expected = expectedYaw*expectedPitch*expectedRoll;
        arma::Mat<double>::fixed<3, 3> expected = expectedRoll*expectedPitch*expectedYaw;

        for (std::size_t i = 0; i < expected.n_elem; ++i) {
          CHECK(result.at(i) == Approx(expected.at(i)));
        }

    }
  }
}

