#include <optimisationProblem/benchmark/sphereFunction.hpp>

#include <cmath>

#include <helper/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace hop {
  SphereFunction::SphereFunction(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions) {

  }

  double SphereFunction::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    return std::pow(arma::norm(parameter - _translation), 2);
  }
}

CEREAL_REGISTER_TYPE(hop::SphereFunction)
