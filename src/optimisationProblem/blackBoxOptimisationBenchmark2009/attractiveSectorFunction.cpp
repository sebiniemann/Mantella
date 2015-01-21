#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/attractiveSectorFunction.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2009 {
    double AttractiveSectorFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const  {
      arma::Col<double> z = rotationQ_ * (delta_ % (rotationR_ * (parameter - translation_)));
      z.elem(arma::find(z % translation_ > 0.0)) *= 100.0;

      return std::pow(getOscillationTransformation(std::pow(norm(z), 2.0)), 0.9);
    }

    std::string AttractiveSectorFunction::to_string() const  {
      return "AttractiveSectorFunction";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2009::AttractiveSectorFunction)
