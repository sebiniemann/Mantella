#include <mantella>

int main() {
  unsigned int numberOfDimensions = 2;
  mant::bbob::DiscusFunction optimisationProblem(numberOfDimensions);

  unsigned int samplesPerDimension = 1001;
  arma::Mat<double> X = arma::repmat(arma::linspace<arma::Row<double>>(-5, 5, samplesPerDimension), samplesPerDimension, 1);
  arma::Mat<double> Y = arma::repmat(arma::linspace<arma::Col<double>>(-5, 5, samplesPerDimension), 1, samplesPerDimension);
  arma::Mat<double> Z(samplesPerDimension, samplesPerDimension);

  for (std::size_t xIndex = 0; xIndex < X.n_cols; ++xIndex) {
    for (std::size_t yIndex = 0; yIndex < X.n_rows; ++yIndex) {
      Z(xIndex, yIndex) = optimisationProblem.getObjectiveValue({X(xIndex, yIndex), Y(xIndex, yIndex)});
    }
  }

  X.save("X.mat", arma::raw_ascii);
  Y.save("Y.mat", arma::raw_ascii);
  Z.save("Z.mat", arma::raw_ascii);

  return 0;
}