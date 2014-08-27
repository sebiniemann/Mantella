#include <optimisationProblem/benchmark/schwefelFunction.hpp>

#include <cmath>

namespace hop {
  SchwefelFunction::SchwefelFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(std::sqrt(10))) {

  }

  double SchwefelFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> xOpt = arma::abs(4.2096874633 * _one);
    arma::Col<double> xHat = 2.0 * _one % parameter;

    arma::Col<double> zHat(xHat.n_elem);
    zHat.at(0) = xHat.at(0);
    zHat.subvec(1, zHat.n_elem - 1) = xHat.subvec(1, zHat.n_elem - 1) + 0.25 * (xHat.subvec(0, xHat.n_elem - 2) - xOpt.subvec(0, xOpt.n_elem - 2));

    arma::Col<double> z = 100.0 * (_delta % (zHat - xOpt) + xOpt);

    return 0.01 * (418.9828872724339 - arma::mean(z % arma::sin(arma::sqrt(arma::abs(z))))) + 100.0 * getPenality(z / 100.0);
  }
}
