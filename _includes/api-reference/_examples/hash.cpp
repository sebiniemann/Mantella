#include <mantella>
#include <map>

int main() {
  unsigned int numberOfDimensions = 5;
  unsigned int numerOfSamples = 100000;
  // All samples are within [-5, 5]^N
  arma::Mat<double> samples = arma::randu<arma::Mat<double>>(numberOfDimensions, numerOfSamples) * 10 - 5;

  std::map<std::size_t, unsigned int> collisionsPerHash;
  mant::Hash<double> hashGenerator;
  
  for (std::size_t n = 0; n < samples.n_cols; ++n) {
    std::size_t hash = hashGenerator(sample.col(n));
    const auto& hashPosition = collisionsPerHash.find(hash)
    if(hashPosition != collisionsPerHash.end()) {
      ++hashPosition->second;
    } else {
      collisionsPerHash.insert({hash, 1});
    }
  }
  
  for () {
  
  }

  X.save("hash.mat", arma::raw_ascii);
  Y.save("collisions.mat", arma::raw_ascii);
  return 0;
}