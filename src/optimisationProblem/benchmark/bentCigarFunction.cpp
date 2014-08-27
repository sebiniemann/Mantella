#include <optimisationProblem/benchmark/bentCigarFunction.hpp>

namespace hop {
  BentCigarFunction::BentCigarFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  double BentCigarFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = arma::square(_rotationR * getAsymmetricTransformation(0.5, _rotationR * (parameter - _translation)));
    return z.at(0) + 1000000 * arma::accu(z.subvec(1, z.n_elem - 1));
  }
}
