#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/bentCigarFunction.hpp>

// C++ standard library
#include <cassert>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    BentCigarFunction::BentCigarFunction(
        const arma::uword numberOfDimensions)
      : BlackBoxOptimisationBenchmark(numberOfDimensions) {
      setParameterTranslation(getRandomParameterTranslation());
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    void BentCigarFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    double BentCigarFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);
        
      const arma::Col<double>& z = arma::square(rotationQ_ * getAsymmetricParameter(0.5, rotationQ_ * parameter));
      return z(0) + 1000000.0 * arma::accu(z.tail(z.n_elem - 1));
    }

    std::string BentCigarFunction::toString() const {
      return "bbob_bent_cigar_function";
    }
    
#if defined(SUPPORT_MPI)
    std::vector<double> BentCigarFunction::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();
      
      for(arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }
      
      return serialisedOptimisationProblem;
    }

    void BentCigarFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      rotationQ_.set_size(numberOfDimensions_, numberOfDimensions_);
      for(arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = serialisedOptimisationProblem.back();
        serialisedOptimisationProblem.pop_back();
      }
        
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
