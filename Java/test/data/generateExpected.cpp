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
  std::array<string, 5> matrices = {"zeros", "ones", "eye", "hankel", "hilbert"};

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
  int dimensions[] = {1, 2, 3, 4, 5, 10};
  std::array<string, 5> matrices = {"zeros", "ones", "eye", "hankel", "hilbert"};

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
  expected.save("./expected/TestUtilAlgebraNorm/testNorm.mat", raw_ascii);
}

int main() {
  testUtilAlgebraCross();
  testUtilAlgebraNorm();

  return EXIT_SUCCESS;
}
