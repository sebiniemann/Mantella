#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian21hiPeaksFunction.hpp>

// C++ standard library
#include <cassert>
#include <cmath>
#include <algorithm>
#include <limits>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction(
        const arma::uword numberOfDimensions)
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        weight_(arma::join_cols(arma::Col<double>({10.0}), arma::linspace<arma::Col<double>>(1.1, 9.1, 20))) {
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
      setLocalParameterConditionings(getRandomLocalParameterConditionings());
      setLocalParameterTranslations(getRandomLocalParameterTranslations());
    }

    void GallaghersGaussian21hiPeaksFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    void GallaghersGaussian21hiPeaksFunction::setLocalParameterConditionings(
        const arma::Mat<double>& localParameterConditionings) {
      verify(localParameterConditionings.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(localParameterConditionings.n_cols == 21, "The number of columns must be equal to the number of peaks (21).");

      localParameterConditionings_ = localParameterConditionings;
    }

    void GallaghersGaussian21hiPeaksFunction::setLocalParameterTranslations(
        const arma::Mat<double>& localParameterTranslations) {
      verify(localParameterTranslations.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(localParameterTranslations.n_cols == 21, "The number of columns must be equal to the number of peaks (21).");

      localParameterTranslations_ = localParameterTranslations;
    }

    arma::Mat<double> GallaghersGaussian21hiPeaksFunction::getRandomLocalParameterConditionings() const {
      arma::Col<double> conditions(21);
      conditions(0) = 19.0;
      conditions.tail(conditions.n_elem - 1) = arma::conv_to<arma::Col<double>>::from(getRandomPermutation(conditions.n_elem - 1));

      arma::Mat<double> localParameterConditionings(numberOfDimensions_, conditions.n_elem);
      for (arma::uword n = 0; n < conditions.n_elem; ++n) {
        const double condition = std::pow(1000.0, conditions(n) / 19.0);
        localParameterConditionings.col(n) = getParameterConditioning(condition) / std::sqrt(condition);
      }

      return localParameterConditionings;
    }

    arma::Mat<double> GallaghersGaussian21hiPeaksFunction::getRandomLocalParameterTranslations() const {
      arma::Mat<double> localParameterTranslations = arma::randu<arma::Mat<double>>(numberOfDimensions_, 21) * 9.8 - 4.9;
      localParameterTranslations.col(0) = 0.8 * localParameterTranslations.col(0);

      return localParameterTranslations;
    }

    double GallaghersGaussian21hiPeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);
        
      double maximalValue = std::numeric_limits<double>::lowest();
      for (arma::uword n = 0; n < 21; ++n) {
        const arma::Col<double>& locallyTranslatedParameter = parameter - localParameterTranslations_.col(n);
        maximalValue = std::max(maximalValue, weight_(n) * std::exp(-0.5 / static_cast<double>(numberOfDimensions_) * arma::dot(locallyTranslatedParameter, rotationQ_.t() * arma::diagmat(localParameterConditionings_.col(n)) * rotationQ_ * locallyTranslatedParameter)));
      }

      return std::pow(getOscillatedValue(10.0 - maximalValue), 2.0);
    }

    std::string GallaghersGaussian21hiPeaksFunction::toString() const {
      return "bbob_gallaghers_gaussian_21hi_peaks_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> GallaghersGaussian21hiPeaksFunction::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();
      
      for(arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }
      
      for(arma::uword n = 0; n < localParameterConditionings_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(localParameterConditionings_(n));
      }
      
      for(arma::uword n = 0; n < localParameterTranslations_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(localParameterTranslations_(n));
      }
      
      return serialisedOptimisationProblem;
    }

    void GallaghersGaussian21hiPeaksFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      localParameterTranslations_.set_size(numberOfDimensions_, 21);
      for(arma::uword n = 0; n < localParameterTranslations_.n_elem; ++n) {
        localParameterTranslations_(n) = serialisedOptimisationProblem.back();
        serialisedOptimisationProblem.pop_back();
      }
      
      localParameterConditionings_.set_size(numberOfDimensions_, 21);
      for(arma::uword n = 0; n < localParameterConditionings_.n_elem; ++n) {
        localParameterConditionings_(n) = serialisedOptimisationProblem.back();
        serialisedOptimisationProblem.pop_back();
      }
      
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
