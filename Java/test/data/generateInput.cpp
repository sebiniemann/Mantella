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
#define ARMA_USE_CXX11 // Use C++11 features, such as initialiser lists#include <iostream>// EXIT_SUCCESS#include <string>
using std::to_string;

#include <armadillo>
using arma::Mat;
using arma::zeros;
using arma::ones;
using arma::eye;
using arma::raw_ascii;
using arma::endr;
using arma::join_horiz;
using arma::join_vert;
using arma::datum;

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

  input = {0, 1, 2, datum::eps, datum::inf, 1/12.0 * datum::pi, 1/10.0 * datum::pi, 1/8.0 * datum::pi, 1/6.0 * datum::pi, 1/4.0 * datum::pi, 1/2.0 * datum::pi, datum::pi, 3/2.0 * datum::pi, 2 * datum::pi, 3 * datum::pi, 4 * datum::pi, 5 * datum::pi, 10 * datum::pi, 100 * datum::pi};
  input = join_vert(input, -input);
  input.save("./input/trigonometric.mat", raw_ascii);

  input = {0, 1, 2, datum::eps, datum::inf, datum::e, 12, 123, 1234, 12345, 123456, 1234567, 12345678, 123456789, 0.9, 0.89, 0.789, 0.6789, 0.56789, 0.456789, 0.3456789, 0.23456789, 0.123456789};
  input = join_vert(input, -input);
  input.save("./input/miscellaneous.mat", raw_ascii);

  input << 0 << 1 << 2 << 10 << 5 << 0 << endr
      << 0 << 1 << 2 << 50 << -55 << 1 << endr
      << 0 << 1 << 2 << 20 << 15 << -1 << endr
      << 0 << 1 << 2 << 40 << -45 << datum::eps << endr
      << 0 << 1 << 2 << 30 << 30 << -datum::eps << endr;
  input = join_horiz(input, -input);
  input.save("./input/statistics.mat", raw_ascii);

  input << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << endr
      << 5 << 6 << 7 << 2 << 3 << 5 << 6 << 3 << 1 << 4 << 8 << 9 << endr
      << 16 << 64 << 512 << 128 << 1 << 2 << 4 << 1 << 2 << 4 << 256 << 32 << endr
      << 0.123456789 << 0.9 << 0.456789 << 0.3456789 << 0.89 << 0.789 << 0.6789 << 0.56789 << 0.23456789 << endr
      << 1 << 0.00390625 << 0.5 << 0.125 << 0.015625 << 0.0625 << 0.125 << 0.015625 << 0.0625 << 0.25 << 0.03625 << 0.0078125 << endr
      << 0 << 1 << 0 << -1 << 0 << datum::eps << -1 << 0 << datum::eps << 0 << -datum::eps << 0 << endr
      << 0 << 1 << 0 << -1 << 0 << datum::inf << -1 << 0 << datum::inf << 0 << -datum::inf << 0 << endr;
  input = join_vert(input, -input);
  input.save("./input/series.mat", raw_ascii);

  input << 0 << 1 << 0 << 123 << -1 << 0 << endr
      << 0 << 1 << 1 << 234 << datum::eps << datum::inf << endr
      << 0 << 1 << 0 << 345 << -1 << 0 << endr
      << 0 << 1 << 1 << 456 << -datum::eps << -datum::inf << endr
      << 0 << 1 << 0 << 567 << -1 << 0 << endr;
  input = join_horiz(input, -input);
  input.save("./input/logic.mat", raw_ascii);

  return EXIT_SUCCESS;
}
