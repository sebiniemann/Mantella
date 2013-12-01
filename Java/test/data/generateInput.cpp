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
#define ARMA_USE_CXX11 // Use C++11 features, such as initialiser lists#include <iostream>// EXIT_SUCCESS#include <string>
using std::to_string;

#include <armadillo>
using arma::Mat;
using arma::zeros;
using arma::ones;
using arma::eye;
using arma::raw_ascii;

Mat<double> getHankelMatrix(int numberOfRows, int numberOfColumns) {
  Mat<double> hankelMatrix(numberOfRows, numberOfColumns);

  for (int j = 0; j < numberOfColumns; j++) {
    for (int i = 0; i < numberOfRows; i++) {
      hankelMatrix.at(i, j) = i + j + 1;
    }
  }

  return hankelMatrix;
}

Mat<double> getHilbertMatrix(int numberOfRows, int numberOfColumns) {
  Mat<double> hilbertMatrix(numberOfRows, numberOfColumns);

  for (int j = 0; j < numberOfColumns; j++) {
    for (int i = 0; i < numberOfRows; i++) {
      hilbertMatrix.at(i, j) = 1.0 / (i + j + 1);
    }
  }

  return hilbertMatrix;
}

Mat<double> getNumberedMatrix(int numberOfRows, int numberOfColumns) {
  Mat<double> numberedMatrix(numberOfRows, numberOfColumns);

  for (int j = 0; j < numberOfColumns; j++) {
    for (int i = 0; i < numberOfRows; i++) {
      numberedMatrix.at(i, j) = i + j * numberedMatrix.n_rows;
    }
  }

  return numberedMatrix;
}

int main() {
  Mat<double> input;

  double dimensions[] = {1, 2, 3, 4, 5, 10, 100};

  for (int numberOfRows : dimensions) {
    for (int numberOfColumns : dimensions) {
      input = zeros<Mat<double>>(numberOfRows, numberOfColumns);
      input.save("./input/zeros." + to_string(numberOfRows) + "x" + to_string(numberOfColumns) + ".mat", raw_ascii);

      input = ones<Mat<double>>(numberOfRows, numberOfColumns);
      input.save("./input/ones." + to_string(numberOfRows) + "x" + to_string(numberOfColumns) + ".mat", raw_ascii);

      input = eye<Mat<double>>(numberOfRows, numberOfColumns);
      input.save("./input/eye." + to_string(numberOfRows) + "x" + to_string(numberOfColumns) + ".mat", raw_ascii);

      input = getNumberedMatrix(numberOfRows, numberOfColumns);
      input.save("./input/numbered." + to_string(numberOfRows) + "x" + to_string(numberOfColumns) + ".mat", raw_ascii);

      input = getHankelMatrix(numberOfRows, numberOfColumns);
      input.save("./input/hankel." + to_string(numberOfRows) + "x" + to_string(numberOfColumns) + ".mat", raw_ascii);

      if (numberOfRows < 10 && numberOfColumns < 10) {
        input = getHilbertMatrix(numberOfRows, numberOfColumns);
        input.save("./input/hilbert." + to_string(numberOfRows) + "x" + to_string(numberOfColumns) + ".mat", raw_ascii);
      }
    }
  }

  return EXIT_SUCCESS;
}
