#include <hop_bits/helper/geometry.hpp>

namespace hop {
  arma::Mat<double>::fixed<2, 2> Geometry::get2DRotationMatrix(const double& angle) {
    // TODO Check angles

      double sinAngle = std::sin(angle);
      double cosAngle = std::cos(angle);

      return arma::Mat<double>::fixed<2, 2>({
        cosAngle, -sinAngle,
        sinAngle,  cosAngle
      });
  }

  // TODO Left or right hand side implementation?
  arma::Mat<double>::fixed<3, 3> Geometry::get3DRotationMatrix(const double& rollAngle, const double& pitchAngle, const double& yawAngle) {
    // TODO Check angles

      double sinRollAngle = std::sin(rollAngle);
      double cosRollAngle = std::cos(rollAngle);
      double sinPitchAngle = std::sin(pitchAngle);
      double cosPitchAngle = std::cos(pitchAngle);
      double sinYawAngle = std::sin(yawAngle);
      double cosYawAngle = std::cos(yawAngle);

      // Avoids Rz*Ry*Rx, as this will suffer from singularities.
      return arma::Mat<double>::fixed<3, 3>({
        cosYawAngle * cosPitchAngle, cosYawAngle * sinPitchAngle * sinRollAngle - sinYawAngle * cosRollAngle, cosYawAngle * sinPitchAngle * cosRollAngle + sinYawAngle * sinRollAngle,
        sinYawAngle * cosPitchAngle, sinYawAngle * sinPitchAngle * sinRollAngle + cosYawAngle * cosRollAngle, sinYawAngle * sinPitchAngle * cosRollAngle - cosYawAngle * sinRollAngle,
                     -sinPitchAngle,                                            cosPitchAngle * sinRollAngle,                                            cosPitchAngle * cosRollAngle
      });
  }

  arma::Mat<double> Geometry::getCircleCircleIntersection(const arma::Mat<double>& firstCenter, const arma::Row<double>& firstRadius, const arma::Mat<double>& secondCenter, const arma::Row<double>& secondRadius) {
    // TODO Check inputs

    arma::Row<double> distance = arma::sum(arma::square(secondCenter - firstCenter));
    arma::Row<double> cosine = (arma::square(firstRadius) - arma::square(secondRadius) + distance) / (2 * arma::sqrt(distance));

    arma::Mat<double> normalisedFirstToSecondCenter = arma::normalise(secondCenter - firstCenter);

    arma::Mat<double> intersection = firstCenter;
    arma::Mat<double> temp = arma::sqrt(arma::square(firstRadius) - arma::square(cosine));
    intersection.row(0) += normalisedFirstToSecondCenter.row(0) % cosine + normalisedFirstToSecondCenter.row(1) % temp;
    intersection.row(1) += normalisedFirstToSecondCenter.row(1) % cosine - normalisedFirstToSecondCenter.row(0) % temp;

    return intersection;
  }

//  arma::Mat<double> Geometry::getCircleSphereIntersection(const arma::Mat<double>& circleCenter, const arma::Row<double>& circleRadius, const arma::Mat<double>& circleNormal, const arma::Mat<double>& sphereCenter, const arma::Row<double>& sphereRadius) {
//    for(std::size_t n = 0; n < )
//    arma::Row<double> distance = arma::dot(circleNormal, sphereCenter - circleCenter);


//    arma::Mat<double>

//    arma::Row<double> cosine = (arma::square(firstRadius) - arma::square(secondRadius) + distance) / (2 * arma::sqrt(distance));

//    // TODO distance ist bereits berechnet
//    arma::Mat<double> normalisedFirstToSecondCenter = secondCenter - firstCenter;
//    normalisedFirstToSecondCenter.each_row() /= distance;

//    // TODO Nicht doppelt berechnen (bis auf unitVector ist alles gleich)
//    arma::Mat<double> intersection = firstCenter;
//    arma::Mat<double> temp = arma::sqrt(arma::square(firstRadius) - arma::square(cosine));
//    intersection.row(0) += normalisedFirstToSecondCenter.row(0) % cosine + normalisedFirstToSecondCenter.row(1) % temp;
//    intersection.row(1) += normalisedFirstToSecondCenter.row(1) % cosine - normalisedFirstToSecondCenter.row(0) % temp;

//    return intersection;
//  }
}
