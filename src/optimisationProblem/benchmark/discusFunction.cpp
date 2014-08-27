#include <optimisationProblem/benchmark/discusFunction.hpp>

namespace hop {
  DiscusFunction::DiscusFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  double DiscusFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = arma::square(getOscillationTransformation(_rotationR * (parameter - _translation)));
    return 1000000 * z.at(0) + arma::accu(z.subvec(1, z.n_elem - 1));
  }
}
