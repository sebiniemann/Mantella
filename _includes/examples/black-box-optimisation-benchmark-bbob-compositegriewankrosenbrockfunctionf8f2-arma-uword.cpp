#include <mantella>

int main() {
  unsigned int numberOfDimensions = 2;
  mant::bbob::CompositeGriewankRosenbrockFunctionF8F2 optimisationProblem(numberOfDimensions);

  arma::Col<double> X = arma::linspace<arma::Col<double>>(-5, 5, 10);
  arma::Col<double> Y = arma::linspace<arma::Col<double>>(-5, 5, 10);
  
  arma::Mat<double> Z(Y.n_elem, X.n_elem);
  for (std::size_t xIndex = 0; xIndex < X.n_elem; ++xIndex) {
    for (std::size_t yIndex = 0; yIndex < Y.n_elem; ++yIndex) {
      Z(yIndex, xIndex) = optimisationProblem.getObjectiveValue({X(xIndex), Y(yIndex)});
    }
  }
  
  std::cout << Z << std::endl;

  return 0;
}
