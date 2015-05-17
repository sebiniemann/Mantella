#include <mantella>

int main() {
  unsigned int numberOfSamples = 10000;

  arma::Mat<double> samples(2, 10000);
  mant::ManhattanDistance<double> distanceFunction;
  for (std::size_t n = 0; n < samples.n_cols; ++n) {
    samples.col(n) = distanceFunction.getRandomNeighbour({0, 0}, 1, 2);
  }
  
  samples.save("samples.mat", arma::raw_ascii);

  return 0;
}