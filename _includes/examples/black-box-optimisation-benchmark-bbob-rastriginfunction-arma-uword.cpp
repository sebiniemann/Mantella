#include <mantella>

int main() {
  unsigned int numberOfDimensions = 2;
  mant::bbob::RastriginFunction optimisationProblem(numberOfDimensions);

  // Create a grid within the default boundaries ([-5, 5]^N) with 10 sample points per dimensions.
  arma::Col<double> X = arma::linspace<arma::Col<double>>(-5, 5, 10);
  arma::Col<double> Y = arma::linspace<arma::Col<double>>(-5, 5, 10);
  
  // Sample the optimisation problem along the grid and stores the objective values in Z.
  arma::Mat<double> Z(Y.n_elem, X.n_elem);
  for (std::size_t xIndex = 0; xIndex < X.n_elem; ++xIndex) {
    for (std::size_t yIndex = 0; yIndex < Y.n_elem; ++yIndex) {
      // Stores the function value - each column stands for an x-value and each row for an y-value.
      Z(yIndex, xIndex) = optimisationProblem.getObjectiveValue({Y(yIndex), X(xIndex)});
    }
  }
  
  std::cout << Z << std::endl;

  return 0;
}
