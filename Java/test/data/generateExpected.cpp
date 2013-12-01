/*******************************************************************************
 * Copyright (c) 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
 *
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/MIT
 *******************************************************************************/
// http://arma.sourceforge.net/docs.html#config_hpp
#define ARMA_USE_CXX11 // Use C++11 features, such as initialiser lists
#include <iostream>
// EXIT_SUCCESS
using std::cout;
using std::endl;
#include <array>
using std::array;

#include <string>
using std::string;
using std::to_string;

#include <cmath>
using std::pow;
using std::sin;
using std::cos;
using std::atan2;

#include <armadillo>
using arma::Mat;
using arma::endr;
using arma::raw_ascii;
using arma::cross;

void testAlgebraCross() {
  std::array < string, 5> matrices = {"zeros", "ones", "eye", "hankel", "hilbert"};

  Mat<double> inputA;
  Mat<double> inputB;

  string filenameA;
  string filenameB;

  Mat<double> expected;
  expected.set_size(3, pow(matrices.size(), 2));

  int j = 0;
  for (string matrixA : matrices) {
    for (string matrixB : matrices) {
      filenameA = matrixA + ".3x1.mat";
      inputA.load("./input/" + filenameA);

      filenameB = matrixB + ".3x1.mat";
      inputB.load("./input/" + filenameB);

      expected.col(j++) = cross(inputA, inputB);
    }
  }
  expected.save("./expected/util/TestAlgebraCross/testCross.mat", raw_ascii);
}

void testAlgebraNorm() {
  std::array<double, 7> dimensions = {1, 2, 3, 4, 5, 10, 100};
  std::array < string, 5> matrices = {"zeros", "ones", "eye", "hankel", "hilbert"};

  Mat<double> input;

  Mat<double> expectedp1;
  Mat<double> expectedp2;
  Mat<double> expectedpinf;
  Mat<double> expectedpfro;
  Mat<double> expectedpminf; // -inf
  expectedp1.set_size(1, matrices.size());
  expectedp2.set_size(1, matrices.size());
  expectedpinf.set_size(1, matrices.size());
  expectedpfro.set_size(1, matrices.size());
  expectedpminf.set_size(1, matrices.size());

  for (int numberOfElements : dimensions) {
    string filename = to_string(1) + "x" + to_string(numberOfElements) + ".mat";
    int j = 0;
    for (string matrix : matrices) {
      if (matrix != "hilbert" || numberOfElements < 10) {
        input.load("./input/" + matrix + "." + filename);

        expectedp1.col(j) = {norm(input, 1)};
        expectedp2.col(j) = {norm(input, 2)};
        expectedpinf.col(j) = {norm(input, "inf")};
        expectedpfro.col(j) = {norm(input, "fro")};
        expectedpminf.col(j) = {norm(input, "-inf")};
      } else {
        expectedp1.col(j) = {0};
        expectedp2.col(j) = {0};
        expectedpinf.col(j) = {0};
        expectedpfro.col(j) = {0};
        expectedpminf.col(j) = {0};
      }
      j++;
    }

    expectedp1.save("./expected/util/TestAlgebraNorm/testNorm.p1." + filename, raw_ascii);
    expectedp2.save("./expected/util/TestAlgebraNorm/testNorm.p2." + filename, raw_ascii);
    expectedpinf.save("./expected/util/TestAlgebraNorm/testNorm.pinf." + filename, raw_ascii);
    expectedpfro.save("./expected/util/TestAlgebraNorm/testNorm.pfro." + filename, raw_ascii);
    expectedpminf.save("./expected/util/TestAlgebraNorm/testNorm.p-inf." + filename, raw_ascii);
  }
}

void testGeometry2DRotationMatrix() {
  Mat<double> input;
  input.load("./input/rotation.mat");

  for (double angle : input) {

    double sinAngle = sin(angle);
    double cosAngle = cos(angle);

    Mat<double> expected;
    expected << cosAngle << -sinAngle << endr
        << sinAngle << cosAngle << endr;
    expected.save("./expected/util/TestGeometry2DRotationMatrix/testGeometry2DRotationMatrix." + to_string(angle) + ".mat", raw_ascii);
  }
}

void testGeometryTaitBryanAngles() {
  Mat<double> input;
  input.load("./input/rotation.mat");

  for (double rollAngle : input) {
    for (double pitchAngle : input) {
      for (double yawAngle : input) {
        double sinRollAngle = sin(rollAngle);
        double cosRollAngle = cos(rollAngle);
        double sinPitchAngle = sin(pitchAngle);
        double cosPitchAngle = cos(pitchAngle);
        double sinYawAngle = sin(yawAngle);
        double cosYawAngle = cos(yawAngle);

        Mat<double> expected;
        expected << cosPitchAngle * cosYawAngle << sinRollAngle * sinPitchAngle * cosYawAngle + cosRollAngle * sinYawAngle << cosRollAngle * sinPitchAngle * cosYawAngle - sinRollAngle * sinYawAngle << endr
            << -cosPitchAngle * sinYawAngle << sinRollAngle * sinPitchAngle * sinYawAngle - cosRollAngle * cosYawAngle << cosRollAngle * sinPitchAngle * sinYawAngle + sinRollAngle * cosYawAngle << endr
            << -sinPitchAngle << -sinRollAngle * cosPitchAngle << cosRollAngle * cosPitchAngle << endr;
        expected.save("./expected/util/TestGeometryTaitBryanAngles/testTaitBryanAngles." + to_string(rollAngle) + "." + to_string(pitchAngle) + "." + to_string(yawAngle) + ".mat", raw_ascii);
      }
    }
  }
}

void testSparse() {
  Mat<double> input;
  input.load("./input/series.mat");

  int nnz = 0;
  for(double value : input) {
    if(value != 0) {
      nnz++;
    }
  }
  Mat<double> expected = {(double) nnz};
  expected.save("./expected/util/TestSparse/testNnz.mat", raw_ascii);
}

void testTrigonometry() {
  Mat<double> input;
  input.load("./input/trigonometric.mat");

  Mat<double> expected;
  expected.set_size(1, input.n_cols);
  for(int j = 0; j < input.n_cols; j++) {
    expected.at(j) = atan2(input.at(0, j), input.at(1, j));
  }
  expected.save("./expected/util/TestTrigonometry/testAtan2.mat", raw_ascii);
}

int main() {
  testAlgebraCross();
  testAlgebraNorm();
  testGeometry2DRotationMatrix();
  testGeometryTaitBryanAngles();
  testSparse();
  testTrigonometry();

  return EXIT_SUCCESS;
}
