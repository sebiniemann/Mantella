#include <optimisationProblem/benchmark/discusFunction.hpp>

#include <armadillo>
using arma::accu;
using arma::square;

namespace hop {
  DiscusFunction::DiscusFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  double DiscusFunction::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z = square(getOscillationTransformation(_rotationR * (parameter - _translation)));
    return 1000000 * z.at(0) + accu(z.subvec(1, z.n_elem - 1));
  }
}
