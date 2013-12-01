/*******************************************************************************
 * Copyright 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
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

#include <armadillo>
using arma::Mat;
using arma::raw_ascii;
using arma::cross;

void testUtilAlgebraCross() {
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
  expected.save("./expected/TestUtilAlgebraCross/testCross.mat", raw_ascii);
}

void testUtilAlgebraNorm() {
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

    expectedp1.save("./expected/TestUtilAlgebraNorm/testNorm.p1." + filename, raw_ascii);
    expectedp2.save("./expected/TestUtilAlgebraNorm/testNorm.p2." + filename, raw_ascii);
    expectedpinf.save("./expected/TestUtilAlgebraNorm/testNorm.pinf." + filename, raw_ascii);
    expectedpfro.save("./expected/TestUtilAlgebraNorm/testNorm.pfro." + filename, raw_ascii);
    expectedpminf.save("./expected/TestUtilAlgebraNorm/testNorm.p-inf." + filename, raw_ascii);
  }
}

int main() {
  testUtilAlgebraCross();
  testUtilAlgebraNorm();

  return EXIT_SUCCESS;
}
