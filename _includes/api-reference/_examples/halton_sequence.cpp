#include <mantella>
#include <iostream>

int main() {
  // Halton (quasi random) sequence
  mant::getHaltonSequence({2, 3}, {0, 1}, 1000).save("halton_sequence.mat", arma::raw_ascii);
  
  // Pseudo random sequence
  static_cast<arma::Mat<double>>(arma::randu<arma::Mat<double>>(2, 1000)).save("pseudo_random_sequence.mat", arma::raw_ascii);
  
  return 0;
}