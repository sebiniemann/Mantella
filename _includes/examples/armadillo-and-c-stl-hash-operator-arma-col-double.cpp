#include <mantella>

int main() {
  // This example converts a matrix where each row is a (parameter, objective value) pair, retrieved for example from a CSV-file, ...
  // parameter | objective value
  // parameter | objective value
  // parameter | objective value
  // ...
  arma::Mat<double> matrixShapedData = arma::randu<arma::Mat<double>>(10, 5);
  
  // ... into an unordered set:
  // **Note:** The first pair within the matrix might not be the first returned by the unordered set, as there is nothing like a first one inside the set.
  std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
  for (std::size_t n = 0; n < matrixShapedData.n_rows; ++n) {
    samples.insert({matrixShapedData(n, arma::span(0, 3)), matrixShapedData(n, 4)});
  }
  
  std::size_t n = 0;
  std::cout << "Sample Nr. | Objective value | Parameter\n";
  for (const auto& sample : samples) {
    std::cout << std::setw(10) << ++n << " | " << std::setw(15) << sample.second << " | " << sample.first.t();
  }
  std::cout << std::flush;

  return 0;
}
