package org.onlineoptimisation.util;

import org.armadillojava.Arma;
import org.armadillojava.Col;
import org.armadillojava.Mat;
import org.armadillojava.Op;

public class Geometry {
  public static Mat circleCircleIntersection(Mat center1, Mat radius1, Mat center2, Mat radius2) {
    Col distance = Arma.sqrt(Arma.sum(Col.class, Arma.square(center1.minus(center2)), 1));

    Mat cosine = (Arma.square(radius1).minus(Arma.square(radius2)).plus(Arma.square(distance))).elemDivide(radius2.elemTimes(distance).times(2));

    Mat unitvector = center2.minus(center1);
    unitvector.each_col(Op.ELEMDIVIDE, distance);

    Mat intersection = center1;
    intersection.col(0, Op.PLUS, (unitvector.col(0).elemTimes(cosine).plus(unitvector.col(1).elemTimes(Arma.sqrt(Arma.square(cosine).times(-1).plus(1))))).elemTimes(radius2));
    intersection.col(1, Op.PLUS, (unitvector.col(1).elemTimes(cosine).minus(unitvector.col(0).elemTimes(Arma.sqrt(Arma.square(cosine).times(-1).plus(1))))).elemTimes(radius2));

    return intersection;
  }
  
  public static Mat circleSphereIntersection(Mat center1, Mat radius1, Mat center2, Mat radius2) {
    return circleCircleIntersection(center1, radius1, center2, radius2);
  }
}
