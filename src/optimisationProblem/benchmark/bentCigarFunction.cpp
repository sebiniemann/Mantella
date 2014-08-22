#include <optimisationProblem/benchmark/bentCigarFunction.hpp>

#include <armadillo>
using arma::square;
using arma::accu;

namespace hop {
  BentCigarFunction::BentCigarFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  double BentCigarFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = square(_rotationR * getAsymmetricTransformation(0.5, _rotationR * (parameter - _translation)));
    return z.at(0) + 1000000 * accu(z.subvec(1, z.n_elem - 1));
  }
}
