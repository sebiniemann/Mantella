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

#include <armadillo>
using arma::Mat;
using arma::raw_ascii;
using arma::cross;

void testUtilAlgebra() {
  Mat<double> input;
  input.load("./input/series.mat");

  int dimensions[] = {1, 2, 3, 4, 5, 10};
  std::array<string, 5> matrices = {"zeros", "ones", "eye", "hankel", "hilbert"};

  string filenameA;
  string filenameB;

  for (string matrixA : matrices) {
    for (string matrixB : matrices) {
      filenameA = matrixA + ".3x1.mat";
      inputA.load("./input/" + filenameA);

      filenameB = matrixB + ".3x1.mat";
      inputB.load("./input/" + filenameB);

      expected = cross(inputA, inputB);
      expected.save("./expected/testUtilAlgebra/testCross." + matrixA + "." + matrixB + ".mat", raw_ascii);
    }
  }
}

int main() {
  testUtilAlgebra();

  return EXIT_SUCCESS;
}
